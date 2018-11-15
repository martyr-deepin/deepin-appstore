/*
 * Copyright (C) 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "services/store_daemon_manager.h"

#include <QThread>

#include "dbus/dbus_consts.h"
#include "dbus/dbus_variant/app_version.h"
#include "dbus/dbus_variant/installed_app_info.h"
#include "dbus/dbus_variant/installed_app_timestamp.h"
#include "dbus/lastore_deb_interface.h"
#include "dbus/lastore_job_interface.h"
#include "services/apt_util_worker.h"

#include "package/package_manager.h"
#include "package/apt_package_manager.h"

namespace dstore
{

namespace
{
const char kResultOk[] = "ok";
const char kResultErrName[] = "errorName";
const char kResultErrMsg[] = "errorMsg";
const char kResult[] = "result";
const char kResultName[] = "name";


bool ReadJobInfo(LastoreJobInterface &job_interface,
                 const QString &job,
                 QVariantMap &result)
{
    result.insert("id", job_interface.id());
    result.insert("job", job);
    result.insert("status", job_interface.status());
    result.insert("type", job_interface.type());
    result.insert("speed", job_interface.speed());
    result.insert("progress", job_interface.progress());
    result.insert("description", job_interface.description());
    result.insert("packages", job_interface.packages());
    result.insert("cancelable", job_interface.cancelable());
    result.insert("downloadSize", job_interface.downloadSize());
    result.insert("createTime", job_interface.createTime());
    result.insert("name", job_interface.name());
    QStringList app_names;

    // Package list may container additional language related packages.
    const QStringList pkgs = job_interface.packages();
    if (pkgs.length() >= 1) {
        const QString &package_name = pkgs.at(0);
        auto packageID =  package_name.split(":").first();
        app_names.append(packageID);
    }

    result.insert("names", app_names);

    return (!app_names.isEmpty());
}
}

class StoreDaemonManagerPrivate
{
public:
    StoreDaemonManagerPrivate(StoreDaemonManager *parent) :
        apt_worker_(new AptUtilWorker()),
        apt_worker_thread_(new QThread(parent)),
        deb_interface_(new LastoreDebInterface(
                           kLastoreDebDbusService,
                           kLastoreDebDbusPath,
                           QDBusConnection::sessionBus(),
                           parent)),
        q_ptr(parent)
    {
        auto aptPM = new AptPackageManager(parent);
        pm = new PackageManager(parent);
        pm->registerDpk("deb", aptPM);

        apt_worker_thread_->start();
        apt_worker_->moveToThread(apt_worker_thread_);
    }

    ~StoreDaemonManagerPrivate()
    {
        apt_worker_thread_->quit();
        apt_worker_thread_->wait(3);
    }

    void initConnections();


    PackageManager *pm = nullptr;
    AptUtilWorker *apt_worker_ = nullptr;
    QThread *apt_worker_thread_ = nullptr;
    LastoreDebInterface *deb_interface_ = nullptr;

    StoreDaemonManager *q_ptr;
    Q_DECLARE_PUBLIC(StoreDaemonManager)
};

void StoreDaemonManagerPrivate::initConnections()
{
    Q_Q(StoreDaemonManager);
    q->connect(deb_interface_, &LastoreDebInterface::jobListChanged,
               q, &StoreDaemonManager::onJobListChanged);
}

StoreDaemonManager::StoreDaemonManager(QObject *parent)
    : QObject(parent),
      dd_ptr(new StoreDaemonManagerPrivate(this))
{
    Q_D(StoreDaemonManager);
    this->setObjectName("StoreDaemonManager");

    AppVersion::registerMetaType();
    InstalledAppInfo::registerMetaType();
    InstalledAppTimestamp::registerMetaType();

    d->initConnections();
}

StoreDaemonManager::~StoreDaemonManager()
{
}


void StoreDaemonManager::clearArchives()
{
    Q_D(StoreDaemonManager);
    d->deb_interface_->CleanArchives();
}

void StoreDaemonManager::openApp(const QString &app_name)
{
    Q_D(StoreDaemonManager);
    emit d->apt_worker_->openAppRequest(app_name);
}

bool StoreDaemonManager::isDBusConnected()
{
    Q_D(StoreDaemonManager);
    return  d->deb_interface_->isValid();
}

QVariantMap StoreDaemonManager::cleanJob(const QString &job)
{
    LastoreJobInterface job_interface(kLastoreDebJobService,
                                      job,
                                      QDBusConnection::sessionBus(),
                                      this);
    if (job_interface.isValid()) {
        const QDBusPendingReply<> reply = job_interface.Clean();
        if (reply.isError()) {
            return QVariantMap {
                { kResultOk, false },
                { kResultErrName, reply.error().name() },
                { kResultErrMsg, reply.error().message() },
                { kResult, job}
            };
        } else {
            return QVariantMap {
                { kResultOk, true },
                { kResultErrName, "" },
                { kResultErrMsg, "" },
                { kResult, job}
            };
        }
    } else {
        return QVariantMap {
            { kResultOk, false },
            { kResultErrName, job_interface.lastError().name() },
            { kResultErrMsg, job_interface.lastError().message() },
        };
    }
}

QVariantMap StoreDaemonManager::pauseJob(const QString &job)
{
    LastoreJobInterface job_interface(kLastoreDebJobService,
                                      job,
                                      QDBusConnection::sessionBus(),
                                      this);
    if (job_interface.isValid()) {
        const QDBusPendingReply<> reply = job_interface.Pause();
        if (reply.isError()) {
            return QVariantMap {
                { kResultOk, false },
                { kResultErrName, reply.error().name() },
                { kResultErrMsg, reply.error().message() },
                { kResult, job}
            };
        } else {
            return QVariantMap {
                { kResultOk, true },
                { kResultErrName, "" },
                { kResultErrMsg, "" },
                { kResult, job}
            };
        }
    } else {
        return QVariantMap {
            { kResultOk, false },
            { kResultErrName, job_interface.lastError().name() },
            { kResultErrMsg, job_interface.lastError().message() },
            {
                kResult, QVariantMap {
                    { kResultName, job },
                }
            }
        };
    }
}

QVariantMap StoreDaemonManager::startJob(const QString &job)
{
    LastoreJobInterface job_interface(kLastoreDebJobService,
                                      job,
                                      QDBusConnection::sessionBus(),
                                      this);
    if (job_interface.isValid()) {
        const QDBusPendingReply<> reply = job_interface.Start();
        if (reply.isError()) {
            return QVariantMap {
                { kResultOk, false },
                { kResultErrName, reply.error().name() },
                { kResultErrMsg, reply.error().message() },
                { kResult, job}
            };
        } else {
            return QVariantMap {
                { kResultOk, true },
                { kResultErrName, "" },
                { kResultErrMsg, "" },
                { kResult, job}
            };
        }
    } else {
        return QVariantMap {
            { kResultOk, false },
            { kResultErrName, job_interface.lastError().name() },
            { kResultErrMsg, job_interface.lastError().message() },
            { kResult, job}
        };
    }
}

QVariantMap StoreDaemonManager::installPackage(const QString &app_name,
        const QString &app_local_name)
{
    // NOTE(Shaohua): package name is also set as job_name so that `name`
    // property in JobInfo is referred to package_name.
    Q_D(StoreDaemonManager);
    const QDBusPendingReply<QDBusObjectPath> reply =
        d->deb_interface_->Install(app_local_name, app_name);
    if (reply.isError()) {
        return QVariantMap {
            { kResultOk, false },
            { kResultErrName, reply.error().name() },
            { kResultErrMsg, reply.error().message() },
            { kResult, app_name},
        };
    } else {
        return QVariantMap {
            { kResultOk, true },
            { kResultErrName, "" },
            { kResultErrMsg, "" },
            { kResult, reply.value().path()},
        };
    }
}

QVariantMap StoreDaemonManager::installedPackages()
{
    // TODO: filter install list
    Q_D(StoreDaemonManager);
    auto result = d->pm->ListInstalled();
    return QVariantMap {
        { kResultOk, result.success },
        { kResultErrName, result.errName },
        { kResultErrMsg, result.errMsg },
        { kResult, result.data},
    };
}

QVariantMap StoreDaemonManager::packageDownloadSize(const QString &app_name)
{
    Q_D(StoreDaemonManager);
    const QDBusPendingReply<qlonglong> reply =
        d->deb_interface_->QueryDownloadSize(app_name);
    if (reply.isError()) {
        return QVariantMap {
            { kResultOk, false },
            { kResultErrName, reply.error().name() },
            { kResultErrMsg, reply.error().message() },
        };
    } else {
        const qlonglong size = reply.value();
        return QVariantMap {
            { kResultOk, true },
            { kResultErrName, reply.error().name() },
            { kResultErrMsg, reply.error().message() },
            { kResult, size },
        };
    }
}

QVariantMap StoreDaemonManager::updatePackage(const QString &app_name,
        const QString &app_local_name)
{
    return this->installPackage(app_name, app_local_name);
}

QVariantMap StoreDaemonManager::removePackage(const QString &app_name,
        const QString &app_local_name)
{
    Q_D(StoreDaemonManager);
    const QDBusPendingReply<QDBusObjectPath> reply =
        d->deb_interface_->Remove(app_local_name, app_name);
    if (reply.isError()) {
        return QVariantMap {
            { kResultOk, false },
            { kResultErrName, reply.error().name() },
            { kResultErrMsg, reply.error().message() },
        };
    } else {
        return QVariantMap {
            { kResultOk, true },
            { kResultErrName, "" },
            { kResultErrMsg, "" },
            { kResult, reply.value().path() },
        };
    }
}

QVariantMap StoreDaemonManager::jobList()
{
    // TODO(Shaohua): List flatpak jobs.
    Q_D(StoreDaemonManager);
    const QList<QDBusObjectPath> jobs = d->deb_interface_->jobList();
    QStringList paths;
    for (const QDBusObjectPath &job : jobs) {
        paths.append(job.path());
    }
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, paths }
    };
}

QVariantMap StoreDaemonManager::queryVersions(const QStringList &apps)
{
    Q_D(StoreDaemonManager);
    auto result = d->pm->QueryVersion(apps);
    return QVariantMap {
        { kResultOk, result.success },
        { kResultErrName, result.errName },
        { kResultErrMsg, result.errMsg },
        { kResult, result.data},
    };
}

QVariantMap StoreDaemonManager::queryInstalledTime(const QStringList &apps)
{
    Q_D(StoreDaemonManager);
    auto result = d->pm->QueryInstalledTime(apps);
    return QVariantMap {
        { kResultOk, result.success },
        { kResultErrName, result.errName },
        { kResultErrMsg, result.errMsg },
        { kResult, result.data},
    };
}

QVariantMap StoreDaemonManager::getJobInfo(const QString &job)
{
    QVariantMap result;
    LastoreJobInterface job_interface(kLastoreDebJobService,
                                      job,
                                      QDBusConnection::sessionBus(),
                                      this);
    if (job_interface.isValid()) {
        if (ReadJobInfo(job_interface, job, result)) {
            return QVariantMap {
                { kResultOk, true },
                { kResultErrName, "" },
                { kResultErrMsg, "" },
                { kResult, result },
            };
        } else {
            return QVariantMap {
                { kResultOk, false },
                { kResultErrName, "app name list is empty" },
                { kResultErrMsg, "" },
                { kResult, job },
            };
        }
    } else {
        return QVariantMap {
            { kResultOk, false },
            { kResultErrName, "Invalid job interface" },
            { kResultErrMsg, job_interface.lastError().message() },
            {
                kResult, QVariantMap {
                    { kResultName, job },
                }
            },
        };
    }
}

QVariantMap StoreDaemonManager::getJobsInfo(const QStringList &jobs)
{
    QVariantList jobs_info;
    for (const QString &job : jobs) {
        QVariantMap job_info;
        LastoreJobInterface job_interface(kLastoreDebJobService,
                                          job,
                                          QDBusConnection::sessionBus());
        if (job_interface.isValid()) {
            if (ReadJobInfo(job_interface, job, job_info)) {
                jobs_info.append(job_info);
            } else {
                qWarning() << "Invalid app_names for job:" << job;
            }
        }
    }
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, jobs_info},
    };
}

void StoreDaemonManager::onJobListChanged()
{
    Q_D(StoreDaemonManager);
    const QList<QDBusObjectPath> jobs = d->deb_interface_->jobList();
    QStringList paths;
    for (const QDBusObjectPath &job : jobs) {
        paths.append(job.path());
    }
    emit this->jobListChanged(paths);
}

QVariantMap StoreDaemonManager::fixError(const QString &error_type)
{
    Q_D(StoreDaemonManager);
    const QDBusObjectPath path = d->deb_interface_->FixError(error_type);
    const QString job_path = path.path();
    return QVariantMap {
        { kResultOk, (!job_path.isEmpty()) },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, job_path},
    };
}

AppPackage AppPackage::fromJson(const QByteArray &json)
{

}

QByteArray AppPackage::toJson() const
{

}

}  // namespace dstore
