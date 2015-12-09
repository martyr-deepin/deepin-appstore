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

    this->architectures = this->manager->systemArchitectures().Value<0>();
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

void processJob(Job* job, QVariantMap* info) {
    // ========= Copy Properties =========

    // package Id
    if (!info->contains("packageId")) {
        const auto packages = job->packages().Value<0>();
        if (packages.length() > 0)
            info->insert("packageId", packages[0]);
    }
    if (!info->contains("id")) {
        info->insert("id", job->id().Value<0>());
    }

    // type
    const auto type = job->type().Value<0>();
    info->insert("type", type);

    if (type == "download") {
        info->insert("speed", job->speed().Value<0>());
    }

    // progress
    auto progress = job->progress().Value<0>();
    if (type == "download" || type == "install") {
        progress /= 2.0;
    }
    if (type == "install") {
        progress += 0.50;
    }
    info->insert("progress", progress);
    const auto status = job->status().Value<0>();
    info->insert("status", status);

    info->insert("cancellable", job->cancelable().Value<0>());

    // ========= is pausable? =========
    auto pausable = false;
    if (type == "download") {
        pausable = (status == "ready" ||
                    status == "running");
    } else if (type == "install") {
        pausable = (status == "ready");
    }
    info->insert("pausable", pausable);

    // ========= is startable? =========
    auto startable = false;
    if ((type == "download" && status == "failed") ||
        (type == "install" && status == "failed") ||
        (type == "download" && status == "paused") ||
        (type == "install" && status == "paused")) {
        startable = true;
    }
    info->insert("startable", startable);
    assert(!(startable && pausable));
}

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

            const auto onPropertiesChanged = [this, toInsert]() {
                processJob(toInsert->object, &toInsert->info);
                emit this->jobInfoAnswered(toInsert->info);
                this->aggregateJobInfo();
            };

            toInsert->info.insert("path", path);
            onPropertiesChanged();
            connect(job, &Job::progressChanged, onPropertiesChanged);
            connect(job, &Job::statusChanged, onPropertiesChanged);
            connect(job, &Job::typeChanged, onPropertiesChanged);
            connect(job, &Job::speedChanged, onPropertiesChanged);
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

    const auto length = this->jobPaths.length();
    if (length) {
        this->overallProgress = overallProgress / length;
        emit this->overallProgressChanged(this->overallProgress);
    }
}

void LAStoreBridge::askOverallProgress() {
    emit this->overallProgressChanged(this->overallProgress);
}
