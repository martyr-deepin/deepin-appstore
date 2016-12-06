/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include <cassert>
#include <chrono>
#include <QProcess>

#include "LAStoreBridge.h"
#include "Bridge.h"
#include "common.h"

using namespace dbus::common;
using namespace dbus::objects;
using namespace dbus::objects::com::deepin::lastore;

#ifdef DEBUG_LASTORE
auto debugLastore() -> QDebug {
    return qDebug() << "[LASTORE]" << std::chrono::system_clock::now().time_since_epoch().count();
}
#endif

LAStoreBridge::LAStoreBridge(QObject *parent) : QObject(parent) {
    this->manager = new Manager("system", "com.deepin.lastore", "/com/deepin/lastore", this);
    connect(this->manager, &Manager::jobListChanged,
            this, &LAStoreBridge::onJobListChanged);
    this->onJobListChanged();

    asyncWatcherFactory<QDBusVariant>(
        this->manager->systemArchitectures(),
        [this](QDBusPendingReply<QDBusVariant> reply) {
            this->architectures = qdbus_cast<QStringList>(reply.argumentAt<0>().variant());
#ifdef DEBUG_LASTORE
            debugLastore() << "SystemArchitectures Answered (change)" << this->architectures;
#endif
            Q_EMIT this->systemArchitecturesAnswered(this->architectures);
        }
    );

    connect(this->manager, &Manager::upgradableAppsChanged,
            this, &LAStoreBridge::onUpdatableAppsChanged);
    this->onUpdatableAppsChanged();

    const auto bridge = static_cast<Bridge *>(this->parent());
    connect(bridge, &Bridge::appRegionAnswered,
            [this](const QString region) {
                this->manager->SetRegion(region);
            });
    bridge->askAppRegion();
}

LAStoreBridge::~LAStoreBridge() {
    if (this->manager) {
        delete this->manager;
        this->manager = nullptr;
    }
}

void LAStoreBridge::installApp(const QString& appId) {
#ifdef DEBUG_LASTORE
    debugLastore() << "installApp" << appId;
#endif
    this->manager->InstallPackage("", appId);
}

void LAStoreBridge::onJobListChanged() {
#ifdef DEBUG_LASTORE
    debugLastore() << "fetch JobList";
#endif
    asyncWatcherFactory<QDBusVariant>(
        this->manager->jobList(),
        [this](QDBusPendingReply<QDBusVariant> reply)  {
            const auto res = reply.argumentAt<0>().variant();
            const auto pathsDBus = qdbus_cast<QList<QDBusObjectPath> >(res);

            // cast from list of DBusPath to list of QString
            QList<QString> paths;
            std::transform(pathsDBus.constBegin(), pathsDBus.constEnd(), std::back_inserter(paths),
                           [](const QDBusObjectPath path) {return path.path();});

            // filter out not interested types of jobs: remove for example
            QList<QString> installPaths;
            std::copy_if(paths.constBegin(), paths.constEnd(), std::back_inserter(installPaths),
                         [](const QString path) { return path.contains("install"); });

#ifdef DEBUG_LASTORE
            debugLastore() << "JobPaths Answered (change)" << installPaths;
#endif
            // re-rendering a list of jobs can be very expensive in the WebView, as the current implementation would
            // destroy all the DOM elements(including canvases) and re-create them, even if they were identical.
            if (this->jobPaths != installPaths) {
                this->jobPaths = installPaths;
                Q_EMIT this->jobPathsAnswered(this->jobPaths);
                this->updateJobDict();
                this->aggregateJobInfo();
            }
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
    Q_FOREACH (const auto& path, this->jobPaths) {
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

#ifdef DEBUG_LASTORE
                    debugLastore() << "fetch" << propertyName <<  "for" << toInsert->object->path() << "done";
                    debugLastore() << "     " << toInsert->nCallback << "callback(s) remaining";
#endif

                    if (propertyName == "packages") {
                        // support old interface
                        const auto packages = toInsert->info["packages"].toStringList();
                        if (packages.length()) {
                            toInsert->info.insert("packageId", packages[0]);
                        }
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
                        #ifdef DEBUG_LASTORE
                            debugLastore() << "JobInfo Answered (change)" << toInsert->info["path"].toString() << toInsert->info;
                        #endif
                        Q_EMIT this->jobInfoAnswered(toInsert->info["path"].toString(), toInsert->info);
                        this->aggregateJobInfo();
                    }
                };
            };

            const auto fetchId = [=]() {
                toInsert->nCallback++;
#ifdef DEBUG_LASTORE
                debugLastore() << "fetch id for" << toInsert->object->path();
#endif
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
#ifdef DEBUG_LASTORE
                debugLastore() << "fetch packages for" << toInsert->object->path();
#endif
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
#ifdef DEBUG_LASTORE
                debugLastore() << "fetch progress for" << toInsert->object->path();
#endif
                asyncWatcherFactory<QDBusVariant>(
                        toInsert->object->progress(),
                        onFetchOneFactory<double>(toInsert, "progress"),
                        nullptr,
                        onFetchOneDoneFactory("progress")
                );
            };
            connect(job, &Job::progressChanged, fetchProgress);
            fetchProgress();

            const auto fetchStatus = [=]() {
                toInsert->nCallback++;
#ifdef DEBUG_LASTORE
                debugLastore() << "fetch status for" << toInsert->object->path();
#endif
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
#ifdef DEBUG_LASTORE
                debugLastore() << "fetch type for" << toInsert->object->path();
#endif
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
#ifdef DEBUG_LASTORE
                debugLastore() << "fetch speed for" << toInsert->object->path();
#endif
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
#ifdef DEBUG_LASTORE
                debugLastore() << "fetch cancellable for" << toInsert->object->path();
#endif
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
    Q_FOREACH (const auto& path, paths) {
        if (!this->jobPaths.contains(path)) {
            toRemove << path;
        }
    }

    // remove old jobs
    Q_FOREACH (const auto& path, toRemove) {
        const auto jobCombo = this->jobDict[path];
        if (jobCombo) {
            jobCombo->object->deleteLater();
        }
        this->jobDict.remove(path);
    }
}


void LAStoreBridge::launchApp(const QString& pkgId) {
#ifdef DEBUG_LASTORE
    debugLastore() << "launchApp" << pkgId;
#endif
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

#ifdef DEBUG_LASTORE
    debugLastore() << "DownloadSize Ask" << pkgs;
#endif
    asyncWatcherFactory<long long>(
        this->manager->PackagesDownloadSize(pkgs),
        [this, pkgId](QDBusPendingReply<long long> reply) {
            const auto size = reply.argumentAt<0>();
#ifdef DEBUG_LASTORE
            debugLastore() << "DownloadSize Answered" << pkgId << size;
#endif
            Q_EMIT this->downloadSizeAnswered(pkgId, size);
        }
    );
}

void LAStoreBridge::onUpdatableAppsChanged() {
#ifdef DEBUG_LASTORE
    debugLastore() << "fetch UpdatableApps";
#endif
    asyncWatcherFactory<QDBusVariant>(
        this->manager->upgradableApps(),
        [this](QDBusPendingReply<QDBusVariant> reply)  {
            this->updatableApps = reply.argumentAt<0>().variant().toStringList();
#ifdef DEBUG_LASTORE
            debugLastore() << "UpdatableApps Answered (change)" << this->updatableApps;
#endif
            Q_EMIT this->updatableAppsAnswered(this->updatableApps);
        }
    );
}

void LAStoreBridge::askAppInstalled(const QString& pkgId) {
#ifdef DEBUG_LASTORE
    debugLastore() << "AppInstalled Ask" << pkgId;
#endif
    asyncWatcherFactory<bool>(
        this->manager->PackageExists(pkgId),
        [this, pkgId](QDBusPendingReply<bool> reply) {
#ifdef DEBUG_LASTORE
            debugLastore() << "AppInstalled Answered" << pkgId << reply.argumentAt<0>();
#endif
            Q_EMIT this->appInstalledAnswered(pkgId, reply.argumentAt<0>());
        }
    );
}

void LAStoreBridge::startJob(const QString& jobId) {
#ifdef DEBUG_LASTORE
    debugLastore() << "startJob" << jobId;
#endif
    this->manager->StartJob(jobId);
}

void LAStoreBridge::pauseJob(const QString& jobId) {
#ifdef DEBUG_LASTORE
    debugLastore() << "pauseJob" << jobId;
#endif
    this->manager->PauseJob(jobId);
}

void LAStoreBridge::cancelJob(const QString& jobId) {
#ifdef DEBUG_LASTORE
    debugLastore() << "cancelJob" << jobId;
#endif
    this->manager->CleanJob(jobId);
}

void LAStoreBridge::updateApp(const QString& appId) {
#ifdef DEBUG_LASTORE
    debugLastore() << "updateApp" << appId;
#endif
    this->manager->UpdatePackage("", appId);
    QString exe = "/usr/bin/dde-control-center";
    QStringList args;
    args << "system_info";
    QProcess::startDetached(exe, args);
}

void LAStoreBridge::askJobInfo(const QString& jobPath) {
#ifdef DEBUG_LASTORE
    debugLastore() << "askJobInfo" << jobPath;
#endif
    const auto jobCombo = this->jobDict[jobPath];
    if (jobCombo) {
        const auto jobInfo = jobCombo->info;
        QTimer::singleShot(0, [this, jobPath, jobInfo]() {
#ifdef DEBUG_LASTORE
            debugLastore() << "JobInfo Answered" << jobPath << jobInfo;
#endif
            Q_EMIT this->jobInfoAnswered(jobPath, jobInfo);
        });
    } else {
        qDebug() << "askJobInfo" << "Cannot find" << jobPath;
    }
}

void LAStoreBridge::aggregateJobInfo() {
    QSet<QString> installingAppsSet;
    QSet<QString> runningJobsSet;
    double overallProgress = 0;
    Q_FOREACH(const auto jobCombo, this->jobDict) {
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
#ifdef DEBUG_LASTORE
        debugLastore() << "InstallingApps Answered (change)" << this->installingApps;
#endif
        Q_EMIT this->installingAppsAnswered(this->installingApps);
    }

    if (this->runningJobsSet != runningJobsSet) {
        this->runningJobsSet = runningJobsSet;

        this->runningJobs = QList<QString>::fromSet(runningJobsSet);
#ifdef DEBUG_LASTORE
        debugLastore() << "RunningJobs Answered (change)" << this->runningJobs;
#endif
        Q_EMIT this->runningJobsAnswered(this->runningJobs);
    }

    const auto length = this->jobPaths.length();
    if (length) {
        this->overallProgress = overallProgress / length;
#ifdef DEBUG_LASTORE
        debugLastore() << "OverallProgress Answered (change)" << this->overallProgress;
#endif
        Q_EMIT this->overallProgressAnswered(this->overallProgress);
    }
}

void LAStoreBridge::askOverallProgress() {
#ifdef DEBUG_LASTORE
    debugLastore() << "OverallProgress Ask";
#endif
    QTimer::singleShot(0, [this] {
#ifdef DEBUG_LASTORE
        debugLastore() << "OverallProgress Answered" << this->overallProgress;
#endif
        Q_EMIT this->overallProgressAnswered(this->overallProgress);
    });
}

void LAStoreBridge::askSystemArchitectures() {
#ifdef DEBUG_LASTORE
    debugLastore() << "SystemArchitectures Ask";
#endif
    if (this->architectures.length()) {
        // in WebView, the PromiseFactory won't capture the answer if response comes too fast
        QTimer::singleShot(0, [this]() {
#ifdef DEBUG_LASTORE
            debugLastore() << "SystemArchitectures Answered" << this->architectures;
#endif
            Q_EMIT this->systemArchitecturesAnswered(this->architectures);
        });
    }
}

void LAStoreBridge::askRunningJobs() {
#ifdef DEBUG_LASTORE
    debugLastore() << "RunningJobs Ask";
#endif
    QTimer::singleShot(0, [this]() {
#ifdef DEBUG_LASTORE
        debugLastore() << "RunningJobs Answered" << this->runningJobs;
#endif
        Q_EMIT this->runningJobsAnswered(this->runningJobs);
    });
}

void LAStoreBridge::askInstallingApps() {
#ifdef DEBUG_LASTORE
    debugLastore() << "InstallingApps Ask";
#endif
    QTimer::singleShot(0, [this]() {
#ifdef DEBUG_LASTORE
        debugLastore() << "InstallingApps Answered" << this->installingApps;
#endif
        Q_EMIT this->installingAppsAnswered(this->installingApps);
    });
}

void LAStoreBridge::askUpdatableApps() {
#ifdef DEBUG_LASTORE
    debugLastore() << "UpdatableApps Ask";
#endif
    QTimer::singleShot(0, [this]() {
#ifdef DEBUG_LASTORE
        debugLastore() << "UpdatableApps Answered" << this->updatableApps;
#endif
        Q_EMIT this->updatableAppsAnswered(this->updatableApps);
    });
}

void LAStoreBridge::askJobPaths() {
#ifdef DEBUG_LASTORE
    debugLastore() << "JobPaths Ask";
#endif
    QTimer::singleShot(0, [this]() {
#ifdef DEBUG_LASTORE
        debugLastore() << "JobPaths Answered" << this->jobPaths;
#endif
        Q_EMIT this->jobPathsAnswered(this->jobPaths);
    });
}
