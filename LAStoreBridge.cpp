#include <cassert>
#include <QProcess>

#include "LAStoreBridge.h"
#include "Bridge.h"
#include "common.h"

using namespace dbus::common;
using namespace dbus::objects;
using namespace dbus::objects::com::deepin::lastore;

LAStoreBridge::LAStoreBridge(QObject *parent) : QObject(parent) {
    this->manager = new Manager("system", "com.deepin.lastore", "/com/deepin/lastore", this);
    connect(this->manager, &Manager::jobListChanged,
            this, &LAStoreBridge::onJobListChanged);

    this->onJobListChanged();

    asyncWatcherFactory<QDBusVariant>(
        this->manager->systemArchitectures(),
        [this](QDBusPendingReply<QDBusVariant> reply) {
            this->architectures = qdbus_cast<QStringList>(reply.argumentAt<0>().variant());
            emit this->systemArchitecturesAnswered(this->architectures);
        }
    );

    connect(this->manager, &Manager::upgradableAppsChanged,
            this, &LAStoreBridge::fetchUpdatableApps);
    this->fetchUpdatableApps();

    connect(this, &LAStoreBridge::jobPathsChanged,
            this, &LAStoreBridge::updateJobDict);

    const auto bridge = static_cast<Bridge *>(this->parent());
    this->manager->SetRegion(bridge->getAppRegion());
}

LAStoreBridge::~LAStoreBridge() {
    if (this->manager) {
        delete this->manager;
        this->manager = nullptr;
    }
}

void LAStoreBridge::installApp(const QString& appId) {
    this->manager->InstallPackage("", appId);
}

void LAStoreBridge::onJobListChanged() {
    asyncWatcherFactory<QDBusVariant>(
        this->manager->jobList(),
        [this](QDBusPendingReply<QDBusVariant> reply)  {
            const auto res = reply.argumentAt<0>().variant();
            const auto pathsDBus = qdbus_cast<QList<QDBusObjectPath> >(res);
            QList<QString> paths;
            std::transform(pathsDBus.constBegin(), pathsDBus.constEnd(), std::back_inserter(paths),
                           [](const QDBusObjectPath path) {return path.path();});
            QList<QString> installPaths;
            std::copy_if(paths.constBegin(), paths.constEnd(), std::back_inserter(installPaths),
                         [](const QString path) { return path.contains("install"); });
            this->jobPaths = installPaths;
            emit this->jobPathsChanged();
            this->aggregateJobInfo();
        }
    );
}

template
<typename T> std::function<void (QDBusPendingReply<QDBusVariant> reply)>
    onFetchOneFactory(JobCombo* toInsert, const QString& name) {
    return [toInsert, name](QDBusPendingReply<QDBusVariant> reply) {
        toInsert->info.insert(name, qdbus_cast<T>(reply.argumentAt<0>().variant()));
    };
};

void LAStoreBridge::updateJobDict() {
    // Maintaining one Job object to each job in the job list
    // and keep them in job dict

    // find and insert new jobs
    foreach (const auto& path, this->jobPaths) {
        if (!this->jobDict.contains(path)) {
            const auto job = new Job("system", "com.deepin.lastore", path, this);
            const auto toInsert = new JobCombo();
            toInsert->object = job;
            toInsert->info = QVariantMap();
            toInsert->nCallback = 0;

            toInsert->info.insert("path", path);

            const auto onFetchOneDoneFactory = [=](const QString& propertyName) {
                return [toInsert, this, propertyName] (bool UNUSED(success)) {
                    // dec nCallback
                    toInsert->nCallback--;

                    if (propertyName == "packages") {
                        // support old interface
                        const auto packages = toInsert->info["packages"].toStringList();
                        if (packages.length()) {
                            toInsert->info.insert("packageId", packages[0]);
                        }
                    }

                    if (propertyName == "progress" ||
                        propertyName == "type") {
                        // adjust progress
                        auto progress = toInsert->info["_progress"].toDouble();
                        const auto& type = toInsert->info["type"].toString();
                        if (type == "download" || type == "install") {
                            progress /= 2.0;
                        }
                        if (type == "install") {
                            progress += 0.50;
                        }
                        toInsert->info.insert("progress", progress);
                    }

                    if (propertyName == "status" ||
                        propertyName == "type") {
                        const auto& type = toInsert->info["type"];
                        const auto& status = toInsert->info["status"];
                        // ========= is pausable? =========
                        auto pausable = false;
                        if (type == "download") {
                            pausable = (status == "ready" ||
                                        status == "running");
                        } else if (type == "install") {
                            pausable = (status == "ready");
                        }
                        toInsert->info.insert("pausable", pausable);

                        // ========= is startable? =========
                        auto startable = false;
                        if ((type == "download" && status == "failed") ||
                            (type == "install" && status == "failed") ||
                            (type == "download" && status == "paused") ||
                            (type == "install" && status == "paused")) {
                            startable = true;
                        }
                        toInsert->info.insert("startable", startable);
                        assert(!(startable && pausable));
                    }


                    if (!toInsert->nCallback) {
                        emit this->jobInfoAnswered(toInsert->info);
                        this->aggregateJobInfo();
                    }
                };
            };

            const auto fetchId = [=]() {
                toInsert->nCallback++;
                asyncWatcherFactory<QDBusVariant>(
                        job->id(),
                        onFetchOneFactory<QString>(toInsert, "id"),
                        nullptr,
                        onFetchOneDoneFactory("id")
                );
            };
            // no need to listen to changes
            fetchId();

            const auto fetchPackages = [=]() {
                toInsert->nCallback++;
                asyncWatcherFactory<QDBusVariant>(
                        job->packages(),
                        onFetchOneFactory<QStringList>(toInsert, "packages"),
                        nullptr,
                        onFetchOneDoneFactory("packages")
                );
            };
            // no need to listen to changes
            fetchPackages();

            const auto fetchProgress = [=]() {
                toInsert->nCallback++;
                asyncWatcherFactory<QDBusVariant>(
                        toInsert->object->progress(),
                        onFetchOneFactory<double>(toInsert, "_progress"),
                        nullptr,
                        onFetchOneDoneFactory("progress")
                );
            };
            connect(job, &Job::progressChanged, fetchProgress);
            fetchProgress();

            const auto fetchStatus = [=]() {
                toInsert->nCallback++;
                asyncWatcherFactory<QDBusVariant>(
                        toInsert->object->status(),
                        onFetchOneFactory<QString>(toInsert, "status"),
                        nullptr,
                        onFetchOneDoneFactory("status")
                );
            };
            connect(job, &Job::statusChanged, fetchStatus);
            fetchStatus();

            const auto fetchType = [=]() {
                toInsert->nCallback++;
                asyncWatcherFactory<QDBusVariant>(
                        toInsert->object->type(),
                        onFetchOneFactory<QString>(toInsert, "type"),
                        nullptr,
                        onFetchOneDoneFactory("type")
                );
            };
            connect(job, &Job::typeChanged, fetchType);
            fetchType();

            const auto fetchSpeed = [=]() {
                toInsert->nCallback++;
                asyncWatcherFactory<QDBusVariant>(
                        toInsert->object->speed(),
                        onFetchOneFactory<long long>(toInsert, "speed"),
                        nullptr,
                        onFetchOneDoneFactory("speed")
                );
            };
            connect(job, &Job::speedChanged, fetchSpeed);
            fetchSpeed();

            const auto fetchCancellable = [=]() {
                toInsert->nCallback++;
                asyncWatcherFactory<QDBusVariant>(
                        toInsert->object->cancelable(),
                        onFetchOneFactory<bool>(toInsert, "cancellable"),
                        nullptr,
                        onFetchOneDoneFactory("cancellable")
                );
            };
            connect(job, &Job::cancelableChanged, fetchCancellable);
            fetchCancellable();


            this->jobDict.insert(path, toInsert);
        }
    }

    // find old jobs
    const auto paths = this->jobDict.keys();
    QStringList toRemove;
    foreach (const auto& path, paths) {
        if (!this->jobPaths.contains(path)) {
            toRemove << path;
        }
    }

    // remove old jobs
    foreach (const auto& path, toRemove) {
        const auto jobCombo = this->jobDict[path];
        if (jobCombo) {
            jobCombo->object->deleteLater();
        }
        this->jobDict.remove(path);
    }
}


void LAStoreBridge::launchApp(const QString& pkgId) {
    asyncWatcherFactory<QString>(
        this->manager->PackageDesktopPath(pkgId),
        [this](QDBusPendingReply<QString> reply) {
            const auto path = reply.argumentAt<0>();
            const auto bridge = static_cast<Bridge *>(this->parent());
            bridge->openDesktopFile(path);
        }
    );
}

void LAStoreBridge::askDownloadSize(const QString& pkgId) {
    QList<QString> pkgs;
    pkgs << pkgId;

    asyncWatcherFactory<long long>(
        this->manager->PackagesDownloadSize(pkgs),
        [this, pkgId](QDBusPendingReply<long long> reply) {
            const auto size = reply.argumentAt<0>();
            emit this->downloadSizeAnswered(pkgId, size);
        }
    );
}

void LAStoreBridge::fetchUpdatableApps() {
    asyncWatcherFactory<QDBusVariant>(
        this->manager->upgradableApps(),
        [this](QDBusPendingReply<QDBusVariant> reply)  {
            this->updatableApps = reply.argumentAt<0>().variant().toStringList();
            emit this->updatableAppsChanged();
        }
    );
}

void LAStoreBridge::askAppInstalled(const QString& pkgId) {
    asyncWatcherFactory<bool>(
        this->manager->PackageExists(pkgId),
        [this, pkgId](QDBusPendingReply<bool> reply) {
            emit this->appInstalledAnswered(pkgId, reply.argumentAt<0>());
        }
    );
}

void LAStoreBridge::startJob(const QString& jobId) {
    this->manager->StartJob(jobId);
}

void LAStoreBridge::pauseJob(const QString& jobId) {
    this->manager->PauseJob(jobId);
}

void LAStoreBridge::cancelJob(const QString& jobId) {
    this->manager->CleanJob(jobId);
}

void LAStoreBridge::updateApp(const QString& appId) {
    this->manager->UpdatePackage("", appId);
    QString exe = "/usr/bin/dde-control-center";
    QStringList args;
    args << "system_info";
    QProcess::startDetached(exe, args);
}

void LAStoreBridge::askJobInfo(const QString& jobPath) {
    const auto jobCombo = this->jobDict[jobPath];
    if (jobCombo) {
        emit this->jobInfoAnswered(jobCombo->info);
    } else {
        qDebug() << "askJobInfo" << "Cannot find" << jobPath;
    }
}

void LAStoreBridge::aggregateJobInfo() {
    QSet<QString> installingAppsSet;
    QSet<QString> runningJobsSet;
    double overallProgress = 0;
    foreach(const auto jobCombo, this->jobDict) {
        if (!jobCombo) {
            // invalid ones, entries in debug page, for instance
            continue;
        }
        // find out which jobs are considered installing jobs
        const auto type = jobCombo->info["type"].toString();
        if (type == "install" || type == "download") {
            const auto status = jobCombo->info["status"].toString();
            if (status != "failed") {
                installingAppsSet << jobCombo->info["packageId"].toString();
            }
            if (status == "running") {
                runningJobsSet << jobCombo->info["packageId"].toString();
            }
        }

        // find out the overProgress
        overallProgress += jobCombo->info["progress"].toDouble();
    }

    // apply
    if (this->installingAppsSet != installingAppsSet) {
        this->installingAppsSet = installingAppsSet;

        this->installingApps = QList<QString>::fromSet(installingAppsSet);
        emit this->installingAppsChanged();
    }

    if (this->runningJobsSet != runningJobsSet) {
        this->runningJobsSet = runningJobsSet;

        this->runningJobs = QList<QString>::fromSet(runningJobsSet);
        emit this->runningJobsAnswered(this->runningJobs);
    }

    const auto length = this->jobPaths.length();
    if (length) {
        this->overallProgress = overallProgress / length;
        emit this->overallProgressChanged(this->overallProgress);
    }
}

void LAStoreBridge::askOverallProgress() {
    emit this->overallProgressChanged(this->overallProgress);
}

void LAStoreBridge::askSystemArchitectures() {
    if (this->architectures.length()) {
        // in WebView, the PromiseFactory won't capture the answer if response comes too fast
        QTimer::singleShot(0, [this]() {
            emit this->systemArchitecturesAnswered(this->architectures);
        });
    }
}

void LAStoreBridge::askRunningJobs() {
    QTimer::singleShot(0, [this]() {
        emit this->runningJobsAnswered(this->runningJobs);
    });
}
