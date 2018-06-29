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
#include "services/dpk_link_validation.h"

namespace dstore {

namespace {

const char kResultOk[] = "ok";
const char kResultErrName[] = "errorName";
const char kResultErrMsg[] = "errorMsg";
const char kResult[] = "result";
const char kResultName[] = "name";
const char kResultValue[] = "value";

void ReadJobInfo(LastoreJobInterface& job_interface,
                 const QString& job,
                 QMultiHash<QString, QString>& deb_names,
                 QVariantMap& result) {
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
    const QString& pkg = pkgs.at(0);
    // TODO(Shaohua): Also check flatpak list.
    if (deb_names.contains(pkg)) {
      app_names = deb_names.values(pkg);
    } else {
      qWarning() << Q_FUNC_INFO << "pkg not found:" << pkg;
    }
  }
  if (app_names.length() != 1) {
    qCritical() << Q_FUNC_INFO << "app name invalid: " << app_names << job;
  }
  result.insert("names", app_names);
}

}  // namespace

StoreDaemonManager::StoreDaemonManager(QObject* parent)
    : QObject(parent),
      apps_(),
      deb_names_(),
      flatpak_names_(),
      apt_worker_(new AptUtilWorker()),
      apt_worker_thread_(new QThread(this)),
      deb_interface_(new LastoreDebInterface(
          kLastoreDebDbusService,
          kLastoreDebDbusPath,
          QDBusConnection::sessionBus(),
          this)) {
  this->setObjectName("StoreDaemonManager");

  AppVersion::registerMetaType();
  InstalledAppInfo::registerMetaType();
  InstalledAppTimestamp::registerMetaType();

  apt_worker_thread_->start();
  apt_worker_->moveToThread(apt_worker_thread_);

  this->initConnections();
}

StoreDaemonManager::~StoreDaemonManager() {
  apt_worker_thread_->quit();
  apt_worker_thread_->wait(3);
}

void StoreDaemonManager::initConnections() {
  connect(this, &StoreDaemonManager::clearArchivesRequest,
          this, &StoreDaemonManager::clearArchives);
  connect(this, &StoreDaemonManager::openAppRequest,
          apt_worker_, &AptUtilWorker::openAppRequest);

  connect(this, &StoreDaemonManager::isDbusConnectedRequest,
          this, &StoreDaemonManager::isDBusConnected);

  connect(this, &StoreDaemonManager::startJobRequest,
          this, &StoreDaemonManager::startJob);
  connect(this, &StoreDaemonManager::pauseJobRequest,
          this, &StoreDaemonManager::pauseJob);
  connect(this, &StoreDaemonManager::cleanJobRequest,
          this, &StoreDaemonManager::cleanJob);

  connect(this, &StoreDaemonManager::installPackageRequest,
          this, &StoreDaemonManager::installPackage);
  connect(this, &StoreDaemonManager::removePackageRequest,
          this, &StoreDaemonManager::removePackage);
  connect(this, &StoreDaemonManager::updatePackageRequest,
          this, &StoreDaemonManager::updatePackage);

  connect(this, &StoreDaemonManager::packageDownloadSizeRequest,
          this, &StoreDaemonManager::packageDownloadSize);

  connect(this, &StoreDaemonManager::installedPackagesRequest,
          this, &StoreDaemonManager::installedPackages);

  connect(this, &StoreDaemonManager::queryVersionsRequest,
          this, &StoreDaemonManager::queryVersions);
  connect(this, &StoreDaemonManager::queryInstalledTimeRequest,
          this, &StoreDaemonManager::queryInstalledTime);

  connect(this, &StoreDaemonManager::jobListRequest,
          this, &StoreDaemonManager::jobList);
  connect(this, &StoreDaemonManager::getJobInfoRequest,
          this, &StoreDaemonManager::getJobInfo);
  connect(this, &StoreDaemonManager::getJobsInfoRequest,
          this, &StoreDaemonManager::getJobsInfo);


  connect(deb_interface_, &LastoreDebInterface::jobListChanged,
          this, &StoreDaemonManager::onJobListChanged);
}

void StoreDaemonManager::updateAppList(const AppSearchRecordList& app_list) {
  apps_.clear();

  QStringList deb_app_names;

  for (const AppSearchRecord& app : app_list) {
    const QString& app_name = app.name;
    const QStringList deb_names = GetDebNames(app.package_uris);
    const QStringList flatpak_names = GetFlatpakNames(app.package_uris);

    for (const QString deb_name : deb_names) {
      deb_app_names.append(deb_name);
      deb_names_.insert(deb_name, app_name);
    }
    for (const QString flatpak_name : flatpak_names) {
      flatpak_names_.insert(flatpak_name, app_name);
    }

    AppSearchRecord ext_app = app;
    ext_app.debs =  {deb_names};
    ext_app.flatpaks = {flatpak_names};
    apps_.insert(app_name, ext_app);
  }

  const QDBusPendingReply<AppVersionList> version_reply =
      deb_interface_->QueryVersion(deb_app_names);

  if (version_reply.isError()) {
    qCritical() << Q_FUNC_INFO << version_reply.error();
  } else {
    AppSearchRecordList existed_app_list;
    QSet<QString> existed_app_names;

    const AppVersionList version_list = version_reply.value();
    for (const AppVersion& version : version_list) {
      QString pkg_name = version.pkg_name;
      const int arch_idx = pkg_name.indexOf(':');
      if (arch_idx > 0) {
        pkg_name = pkg_name.left(arch_idx);
      }
      deb_app_names.removeOne(pkg_name);

      if (deb_names_.contains(pkg_name)) {
        const QStringList app_names = deb_names_.values(pkg_name);
        for (const QString& app_name : app_names) {
          const AppSearchRecord& app = apps_[app_name];
          if (!existed_app_names.contains(app_name)) {
            existed_app_names.insert(app_name);
            existed_app_list.append(app);
          }
          if (arch_idx > 0) {
            deb_names_.insert(version.pkg_name, app_name);
          }
        }
      }
    }
    emit this->onAppListUpdated(existed_app_list);
  }
}

void StoreDaemonManager::clearArchives() {
  deb_interface_->CleanArchives();
}

void StoreDaemonManager::isDBusConnected() {
  const bool state = deb_interface_->isValid();
  emit this->isDbusConnectedReply(state);
}

void StoreDaemonManager::cleanJob(const QString& job) {
  LastoreJobInterface job_interface(kLastoreDebJobService,
                                    job,
                                    QDBusConnection::sessionBus(),
                                    this);
  if (job_interface.isValid()) {
    const QDBusPendingReply<> reply = job_interface.Clean();
    if (reply.isError()) {
      emit this->cleanJobReply(QVariantMap {
          { kResultOk, false },
          { kResultErrName, reply.error().name() },
          { kResultErrMsg, reply.error().message() },
          { kResult, QVariantMap {
              { kResultName, job },
          }}
      });
    } else {
      emit this->cleanJobReply(QVariantMap {
          { kResultOk, true },
          { kResultErrName, "" },
          { kResultErrMsg, "" },
          { kResult, QVariantMap {
              { kResultName, job },
          }}
      });
    }
  } else {
    emit this->cleanJobReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, job_interface.lastError().name() },
        { kResultErrMsg, job_interface.lastError().message() },
        { kResult, QVariantMap {
            { kResultName, job },
        }}
    });
  }
}

void StoreDaemonManager::pauseJob(const QString& job) {
  LastoreJobInterface job_interface(kLastoreDebJobService,
                                    job,
                                    QDBusConnection::sessionBus(),
                                    this);
  if (job_interface.isValid()) {
    const QDBusPendingReply<> reply = job_interface.Pause();
    if (reply.isError()) {
      emit this->pauseJobReply(QVariantMap {
          { kResultOk, false },
          { kResultErrName, reply.error().name() },
          { kResultErrMsg, reply.error().message() },
          { kResult, QVariantMap {
              { kResultName, job },
          }}
      });
    } else {
      emit this->pauseJobReply(QVariantMap {
          { kResultOk, true },
          { kResultErrName, "" },
          { kResultErrMsg, "" },
          { kResult, QVariantMap {
              { kResultName, job },
          }}
      });
    }
  } else {
    emit this->pauseJobReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, job_interface.lastError().name() },
        { kResultErrMsg, job_interface.lastError().message() },
        { kResult, QVariantMap {
            { kResultName, job },
        }}
    });
  }
}

void StoreDaemonManager::startJob(const QString& job) {
  LastoreJobInterface job_interface(kLastoreDebJobService,
                                    job,
                                    QDBusConnection::sessionBus(),
                                    this);
  if (job_interface.isValid()) {
    const QDBusPendingReply<> reply = job_interface.Start();
    if (reply.isError()) {
      emit this->startJobReply(QVariantMap {
          { kResultOk, false },
          { kResultErrName, reply.error().name() },
          { kResultErrMsg, reply.error().message() },
          { kResult, QVariantMap {
              { kResultName, job },
          }}
      });
    } else {
      emit this->startJobReply(QVariantMap {
          { kResultOk, true },
          { kResultErrName, "" },
          { kResultErrMsg, "" },
          { kResult, QVariantMap {
              { kResultName, job },
          }}
      });
    }
  } else {
    emit this->startJobReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, job_interface.lastError().name() },
        { kResultErrMsg, job_interface.lastError().message() },
        { kResult, QVariantMap {
            { kResultName, job },
        }}
    });
  }
}

void StoreDaemonManager::installPackage(const QString& app_name,
                                        const QString& app_local_name) {
  // NOTE(Shaohua): package name is also set as job_name so that `name`
  // property in JobInfo is referred to package_name.

  if (this->hasFlatPak(app_name)) {
    emit this->installPackageReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, "flatpak not supported" },
        { kResultErrMsg, "flatpak not supported" },
        { kResult, QVariantMap {
            { kResultName, app_name },
        }},
    });
  } else if (this->hasDebPkg(app_name)) {
    const QDBusPendingReply<QDBusObjectPath> reply =
        deb_interface_->Install(app_local_name, app_name);
    if (reply.isError()) {
      emit this->installPackageReply(QVariantMap {
          { kResultOk, false },
          { kResultErrName, reply.error().name() },
          { kResultErrMsg, reply.error().message() },
          { kResult, QVariantMap {
              { kResultName, app_name },
          }},
      });
    } else {
      emit this->installPackageReply(QVariantMap {
          { kResultOk, true },
          { kResultErrName, "" },
          { kResultErrMsg, "" },
          { kResult, QVariantMap {
              { kResultName, app_name },
              { kResultValue, reply.value().path() },
          }},
      });
    }
  } else {
    qCritical() << Q_FUNC_INFO << "Invalid app: " << app_name;
    emit this->installPackageReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, "Invalid app" },
        { kResultErrMsg, "Invalid app" },
        { kResult, QVariantMap {
            { kResultName, app_name },
        }},
    });
  }
}

void StoreDaemonManager::installedPackages() {
  // TODO(Shaohua): List flatpak packages.
  const QDBusPendingReply<InstalledAppInfoList> reply =
      deb_interface_->ListInstalled();
  if (reply.isError()) {
    emit this->installedPackagesReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariantMap {
            { kResultName, "" },
        }},
    });
  } else {
    const InstalledAppInfoList list = reply.value();
    QVariantList result;
    for (const InstalledAppInfo& info : list) {
      if (deb_names_.contains(info.pkg_name)) {
        // Convert deb package name to appName.
        const QString& app_name = deb_names_.value(info.pkg_name);
        result.append(QVariantMap {
            { "name", app_name },
            { "version", info.version },
            { "size", info.size },
        });
      }
    }

    emit this->installedPackagesReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
            { kResultName, "" },
            { kResultValue, result },
        }},
    });
  }
}

void StoreDaemonManager::packageDownloadSize(const QString& app_name) {

  if (this->hasFlatPak(app_name)) {
    emit this->packageDownloadSizeReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, "flatpak not supported" },
        { kResultErrMsg, "flatpak not supported" },
        { kResult, QVariantMap {
            { kResultName, app_name },
        }},
    });
  } else if (this->hasDebPkg(app_name)) {
    const QDBusPendingReply<qlonglong> reply =
        deb_interface_->QueryDownloadSize(app_name);
    if (reply.isError()) {
      emit this->packageDownloadSizeReply(QVariantMap {
          { kResultOk, false },
          { kResultErrName, reply.error().name() },
          { kResultErrMsg, reply.error().message() },
          { kResult, QVariantMap {
              { kResultName, app_name },
          }},
      });
    } else {
      const qlonglong size = reply.value();
      emit this->packageDownloadSizeReply(QVariantMap {
          { kResultOk, true },
          { kResultErrName, reply.error().name() },
          { kResultErrMsg, reply.error().message() },
          { kResult, QVariantMap {
              { kResultName, app_name },
              { kResultValue, size },
          }},
      });
    }
  } else {
    qCritical() << Q_FUNC_INFO << "Invalid app: " << app_name;
    emit this->packageDownloadSizeReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, "Invalid app" },
        { kResultErrMsg, "Invalid app" },
        { kResult, QVariantMap {
            { kResultName, app_name },
        }},
    });
  }
}

void StoreDaemonManager::updatePackage(const QString& app_name,
                                       const QString& app_local_name) {
  this->installPackage(app_name, app_local_name);
}

void StoreDaemonManager::removePackage(const QString& app_name,
                                       const QString& app_local_name) {
  if (this->hasFlatPak(app_name)) {
    emit this->removePackageReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, "flatpak not supported" },
        { kResultErrMsg, "flatpak not supported" },
        { kResult, QVariantMap {
            { kResultName, app_name },
        }},
    });
  } else if (this->hasDebPkg(app_name)) {
    const QDBusPendingReply<QDBusObjectPath> reply =
        deb_interface_->Remove(app_local_name, app_name);
    if (reply.isError()) {
      emit this->removePackageReply(QVariantMap {
          { kResultOk, false },
          { kResultErrName, reply.error().name() },
          { kResultErrMsg, reply.error().message() },
          { kResult, QVariantMap {
              { kResultName, app_name },
          }},
      });
    } else {
      emit this->removePackageReply(QVariantMap {
          { kResultOk, true },
          { kResultErrName, "" },
          { kResultErrMsg, "" },
          { kResult, QVariantMap {
              { kResultName, app_name },
              { kResultValue, reply.value().path() },
          }},
      });
    }
  } else {
    qCritical() << Q_FUNC_INFO << "Invalid app: " << app_name;
    emit this->removePackageReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, "Invalid app_name" },
        { kResultErrMsg, "Invalid app_name" },
        { kResult, QVariantMap {
            { kResultName, app_name },
        }},
    });
  }
}

void StoreDaemonManager::jobList() {
  // TODO(Shaohua): List flatpak jobs.

  const QList<QDBusObjectPath> jobs = deb_interface_->jobList();
  QStringList paths;
  for (const QDBusObjectPath& job : jobs) {
    paths.append(job.path());
  }
  emit this->jobListReply(QVariantMap {
      { kResultOk, true },
      { kResultErrName, "" },
      { kResultErrMsg, "" },
      { kResult, QVariantMap {
          { kResultName, "", },
          { kResultValue, paths },
      }}
  });
}

void StoreDaemonManager::queryVersions(const QString& task_id,
                                       const QStringList& apps) {
  // Remap app name.
  QStringList deb_names;
  for (const QString& app : apps) {
    if (apps_.contains(app)) {
      deb_names.append(apps_[app].debs);
    }
  }

  const QDBusPendingReply<AppVersionList> version_reply =
      deb_interface_->QueryVersion(deb_names);

  if (version_reply.isError()) {
    emit this->queryVersionsReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, version_reply.error().name() },
        { kResultErrMsg, version_reply.error().message() },
        { kResult, QVariantMap {
            { kResultName, task_id },
        }},
    });
  } else {
    const AppVersionList version_list = version_reply.value();
    QVariantList version_vars;
    for (const AppVersion& version : version_list) {
      if (deb_names_.contains(version.pkg_name)) {
        const QStringList& app_names = deb_names_.values(version.pkg_name);
        for (const QString& app_name : app_names) {
          version_vars.append(QVariantMap {
              { "name", app_name },
              { "localVersion", version.installed_version },
              { "remoteVersion", version.remote_version },
              { "upgradable", version.upgradable },
          });
        }
      }
    }

    emit this->queryVersionsReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
            { kResultName, task_id },
            { kResultValue, version_vars },
        }},
    });
  }
}

void StoreDaemonManager::queryInstalledTime(const QString& task_id,
                                            const QStringList& apps) {
  // Remap app name.
  QStringList deb_names;
  for (const QString& app : apps) {
    if (apps_.contains(app)) {
      deb_names.append(apps_[app].debs);
    }
  }

  const QDBusPendingReply<InstalledAppTimestampList> timestamp_reply =
      deb_interface_->QueryInstallationTime(deb_names);

  if (timestamp_reply.isError()) {
    emit this->queryInstalledTimeReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, timestamp_reply.error().name() },
        { kResultErrMsg, timestamp_reply.error().message() },
        { kResult, QVariantMap {
            { kResultName, task_id },
        }},
    });
  } else {
    const InstalledAppTimestampList timestamp_list = timestamp_reply.value();
    QVariantList result;
    for (const InstalledAppTimestamp& timestamp : timestamp_list) {
      if (deb_names_.contains(timestamp.pkg_name)) {
        const QStringList& app_names = deb_names_.values(timestamp.pkg_name);
        for (const QString& app_name : app_names) {
          result.append(QVariantMap {
              { "app", app_name },
              { "time", timestamp.timestamp },
          });
        }
      }
    }

    emit this->queryInstalledTimeReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
            { kResultName, task_id },
            { kResultValue, result },
        }},
    });
  }
}

void StoreDaemonManager::getJobInfo(const QString& job) {
  QVariantMap result;
  LastoreJobInterface job_interface(kLastoreDebJobService,
                                    job,
                                    QDBusConnection::sessionBus(),
                                    this);
  if (job_interface.isValid()) {
    ReadJobInfo(job_interface, job, deb_names_, result);

    emit this->getJobInfoReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
            { kResultName, job },
            { kResultValue, result },
        }},
    });
  } else {
    emit this->getJobInfoReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, "Invalid job interface" },
        { kResultErrMsg, job_interface.lastError().message() },
        { kResult, QVariantMap {
            { kResultName, job },
        }},
    });
  }
}

bool StoreDaemonManager::hasDebPkg(const QString& app_name) const {
  return (apps_.contains(app_name) && !apps_.value(app_name).debs.isEmpty());
}

bool StoreDaemonManager::hasFlatPak(const QString& app_name) const {
  return (apps_.contains(app_name) && !apps_.value(app_name).flatpaks.isEmpty());
}

void StoreDaemonManager::getJobsInfo(const QString& task_id,
                                     const QStringList& jobs) {
  QVariantList jobs_info;
  for (const QString& job : jobs) {
    QVariantMap job_info;
    LastoreJobInterface job_interface(kLastoreDebJobService,
                                      job,
                                      QDBusConnection::sessionBus(),
                                      this);
    if (job_interface.isValid()) {
      ReadJobInfo(job_interface, job, deb_names_, job_info);
      jobs_info.append(job_info);
    }
  }
  emit this->getJobsInfoReply(QVariantMap {
      { kResultOk, true },
      { kResultErrName, "" },
      { kResultErrMsg, "" },
      { kResult, QVariantMap {
          { kResultName, task_id },
          { kResultValue, jobs_info },
      }},
  });
}

void StoreDaemonManager::onJobListChanged() {
  const QList<QDBusObjectPath> jobs = deb_interface_->jobList();
  QStringList paths;
  for (const QDBusObjectPath& job : jobs) {
    paths.append(job.path());
  }
  emit this->jobListChanged(paths);
}

}  // namespace dstore