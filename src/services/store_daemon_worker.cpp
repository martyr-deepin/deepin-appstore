/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
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

#include "services/store_daemon_worker.h"

#include <QDebug>
#include <QDBusPendingReply>

#include "dbus/dbus_consts.h"
#include "dbus/lastore_job_interface.h"
#include "dbus/lastore_manager_interface.h"
#include "dbus/lastore_updater_interface.h"
#include "base/launcher.h"

namespace dstore {

namespace {

const char kResultOk[] = "ok";
const char kResultErrName[] = "errorName";
const char kResultErrMsg[] = "errorMsg";
const char kResult[] = "result";
const char kResultName[] = "name";
const char kResultValue[] = "value";

}  // namespace

StoreDaemonWorker::StoreDaemonWorker(QObject* parent)
    : QObject(parent),
      manager_(new LastoreManagerInterface(
          kLastoreManagerService,
          kLastoreManagerPath,
          QDBusConnection::systemBus(),
          this)),
      updater_(new LastoreUpdaterInterface(
          kLastoreUpdaterService,
          kLastoreUpdaterPath,
          QDBusConnection::systemBus(),
          this)) {
  this->setObjectName("StoreDaemonWorker");
  AppUpdateInfo::registerMetaType();
  LocaleMirrorSource::registerMetaType();

  this->initConnections();
}

StoreDaemonWorker::~StoreDaemonWorker() {
}

void StoreDaemonWorker::initConnections() {
  connect(this, &StoreDaemonWorker::isDbusConnectedRequest,
          this, &StoreDaemonWorker::isDBusConnected);

  connect(this, &StoreDaemonWorker::cleanArchivesRequest,
          this, &StoreDaemonWorker::cleanArchives);
  connect(this, &StoreDaemonWorker::cleanJobRequest,
          this, &StoreDaemonWorker::cleanJob);
  connect(this, &StoreDaemonWorker::pauseJobRequest,
          this, &StoreDaemonWorker::pauseJob);
  connect(this, &StoreDaemonWorker::startJobRequest,
          this, &StoreDaemonWorker::startJob);
  connect(this, &StoreDaemonWorker::installPackageRequest,
          this, &StoreDaemonWorker::installPackage);
  connect(this, &StoreDaemonWorker::packageExistsRequest,
          this, &StoreDaemonWorker::packageExists);
  connect(this, &StoreDaemonWorker::packageInstallableRequest,
          this, &StoreDaemonWorker::packageInstallable);
  connect(this, &StoreDaemonWorker::packageDownloadSizeRequest,
          this, &StoreDaemonWorker::packageDownloadSize);
  connect(this, &StoreDaemonWorker::updatePackageRequest,
          this, &StoreDaemonWorker::updatePackage);
  connect(this, &StoreDaemonWorker::removePackageRequest,
          this, &StoreDaemonWorker::removePackage);
  connect(this, &StoreDaemonWorker::upgradableAppsRequest,
          this, &StoreDaemonWorker::upgradableApps);

  connect(this, &StoreDaemonWorker::applicationUpdateInfosRequest,
          this, &StoreDaemonWorker::applicationUpdateInfos);

  connect(this, &StoreDaemonWorker::jobListRequest,
          this, &StoreDaemonWorker::jobList);
  connect(this, &StoreDaemonWorker::getJobInfoRequest,
          this, &StoreDaemonWorker::getJobInfo);

  connect(this, &StoreDaemonWorker::openAppRequest,
          this, &StoreDaemonWorker::openApp);
}

void StoreDaemonWorker::isDBusConnected() {
  const bool state = (manager_->isValid() && updater_->isValid());
  emit this->isDbusConnectedReply(state);
}

void StoreDaemonWorker::cleanArchives() {
  const QDBusPendingReply<QDBusObjectPath> reply = manager_->CleanArchives();
  if (reply.isError()) {
    emit this->cleanArchivesReply(
    QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariant() },
    });
  } else {
    emit this->cleanArchivesReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap{
            { kResultName, "" },
            { kResultValue, reply.value().path() },
          },
        },
    });
  }
}

void StoreDaemonWorker::cleanJob(const QString& job) {
  const QDBusPendingReply<> reply = manager_->CleanJob(job);
  if (reply.isError()) {
    emit this->cleanJobReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariant() },
    });
  } else {
    emit this->cleanArchivesReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap() },
    });
  }
}

//const QString StoreDaemonProxy::distUpgrade() {
//  const QDBusObjectPath path = manager_->DistUpgrade();
//  return path.path();
//}

void StoreDaemonWorker::installPackage(const QString& app_name) {
  // NOTE(Shaohua): package name is also set as job_name so that `name`
  // property in JobInfo is referred to package_name.
  const QDBusPendingReply<QDBusObjectPath> reply =
      manager_->InstallPackage(app_name, app_name);
  if (reply.isError()) {
    emit this->installPackageReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariant() },
    });
  } else {
    emit this->installPackageReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
            { kResultName, app_name },
            { kResultValue, reply.value().path() },
          },
        },
    });
  }
}

//const QString StoreDaemonProxy::packageDesktopPath(const QString& app_name) {
//  return manager_->PackageDesktopPath(app_name);
//}

void StoreDaemonWorker::packageExists(const QString& app_name) {
  const QDBusPendingReply<bool> reply = manager_->PackageExists(app_name);
  if (reply.isError()) {
    emit this->packageExistsReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariantMap {
            { kResultName, app_name },
            { kResultValue, false },
          }
        },
    });
  } else {
    emit this->packageExistsReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap{
            { kResultName, app_name },
            { kResultValue, reply.value() },
          }
        },
    });
  }
}

void StoreDaemonWorker::packageInstallable(const QString& app_name) {
  const QDBusPendingReply<bool> reply = manager_->PackageInstallable(app_name);
  if (reply.isError()) {
    emit this->packageInstallableReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariantMap {
            { kResultName, app_name },
            { kResultValue, false },
          }
        },
    });
  } else {
    emit this->packageInstallableReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap{
            { kResultName, app_name },
            { kResultValue, reply.value() },
          }
        },
    });
  }
}

void StoreDaemonWorker::packageDownloadSize(const QString& app_name) {
  const QDBusPendingReply<qlonglong> reply =
      manager_->PackagesDownloadSize({app_name});
  if (reply.isError()) {
    emit this->packageDownloadSizeReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariantMap {
            { kResultName, app_name },
            { kResultValue, 0 },
          }
        },
    });
  } else {
    emit this->packageDownloadSizeReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap{
            { kResultName, app_name },
            { kResultValue, reply.value() },
          }
        },
    });
  }
}

void StoreDaemonWorker::pauseJob(const QString& job) {
  const QDBusPendingReply<> reply = manager_->PauseJob(job);
  if (reply.isError()) {
    emit this->pauseJobReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariantMap {
            { kResultName, job },
            { kResultValue, "" },
          }
        },
    });
  } else {
    emit this->pauseJobReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap{
            { kResultName, job },
            { kResultValue, "" }
          }
        },
    });
  }
}

//const QString StoreDaemonProxy::prepareDistUpgrade() {
//  const QDBusObjectPath path = manager_->PrepareDistUpgrade();
//  return path.path();
//}
//
//void StoreDaemonProxy::recordLocaleInfo(const QString& language) {
//  manager_->RecordLocaleInfo(language);
//}

void StoreDaemonWorker::startJob(const QString& job) {
  const QDBusPendingReply<> reply = manager_->StartJob(job);
  if (reply.isError()) {
    emit this->startJobReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariantMap {
            { kResultName, job },
            { kResultValue, "" },
          }
        },
    });
  } else {
    emit this->startJobReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
            { kResultName, job },
            { kResultValue, "" },
          }
        },
    });
  }
}

void StoreDaemonWorker::updatePackage(const QString& app_name) {
  const QDBusPendingReply<QDBusObjectPath> reply =
      manager_->UpdatePackage(app_name, app_name);
  if (reply.isError()) {
    emit this->updatePackageReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariant() },
    });
  } else {
    emit this->updatePackageReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap{
            { kResultName, app_name },
            { kResultValue, reply.value().path() },
          }
        },
    });
  }
}

//const QString StoreDaemonProxy::updateSource() {
//  const QDBusObjectPath path = manager_->UpdateSource();
//  return path.path();
//}

void StoreDaemonWorker::removePackage(const QString& app_name) {
  const QDBusPendingReply<QDBusObjectPath> reply =
      manager_->RemovePackage(app_name, app_name);
  if (reply.isError()) {
    emit this->removePackageReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariant() },
    });
  } else {
    emit this->removePackageReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
            { kResultName, app_name },
            { kResultValue, reply.value().path() },
          }
        },
    });
  }
}
//
//void StoreDaemonProxy::setAutoClean(bool enabled) {
//  manager_->SetAutoClean(enabled);
//}
//
//void StoreDaemonProxy::setRegion(const QString& region) {
//  manager_->SetRegion(region);
//}
//
//bool StoreDaemonProxy::autoClean() {
//  return manager_->autoClean();
//}

void StoreDaemonWorker::jobList() {
  auto list = manager_->jobList();
  QStringList result;
  for (const QDBusObjectPath& path : list ) {
    result.append(path.path());
  }

  emit this->jobListReply(QVariantMap {
      { kResultOk, true },
      { kResultErrName, "" },
      { kResultErrMsg, "" },
      { kResult, QVariantMap {
          { kResultName, "" },
          { kResultValue, result },
        }
      },
  });
}

//const QStringList StoreDaemonProxy::systemArchitectures() {
//  return manager_->systemArchitectures();
//}
//
//bool StoreDaemonProxy::systemOnChanging() {
//  return manager_->systemOnChanging();
//}

void StoreDaemonWorker::upgradableApps() {
  const QStringList reply = manager_->upgradableApps();
  emit this->upgradableAppsReply(QVariantMap {
      { kResultOk, true },
      { kResultErrName, "" },
      { kResultErrMsg, "" },
      { kResult, QVariantMap {
          { kResultName, "" },
          { kResultValue, reply },
        }
      },
  });
}

void StoreDaemonWorker::applicationUpdateInfos(const QString& language) {
  const AppUpdateInfoList list = updater_->ApplicationUpdateInfos(language);
  const auto variant_list = AppUpdateInfoListToVariant(list);
  emit this->applicationUpdateInfosReply(QVariantMap {
      { kResultOk, true },
      { kResultErrName, "" },
      { kResultErrMsg, "" },
      { kResult, QVariantMap {
          { kResultName, language },
          { kResultValue, variant_list },
        }
      }
  });
}
//
//const QVariantList StoreDaemonProxy::listMirrorSources(
//    const QString& language) {
//  const LocaleMirrorSourceList mirrors = updater_->ListMirrorSources(language);
//  return LocaleMirrorSourceListToVariant(mirrors);
//}
//
//void StoreDaemonProxy::setAutoCheckUpdates(bool check) {
//  updater_->SetAutoCheckUpdates(check);
//}
//
//void StoreDaemonProxy::setAutoDownloadUpdates(bool update) {
//  updater_->SetAutoDownloadUpdates(update);
//}
//
//void StoreDaemonProxy::setMirrorSource(const QString& id) {
//  updater_->SetMirrorSource(id);
//}
//
//bool StoreDaemonProxy::autoCheckUpdates() {
//  return updater_->autoCheckUpdates();
//}
//
//bool StoreDaemonProxy::autoDownloadUpdates() {
//  return updater_->autoDownloadUpdates();
//}
//
//const QString StoreDaemonProxy::mirrorSource() {
//  return updater_->mirrorSource();
//}

//QStringList StoreDaemonProxy::updatableApps() {
//  return updater_->updatableApps();
//}
//
//QStringList StoreDaemonProxy::updatablePackages() {
//  return updater_->updatablePackages();
//}

void StoreDaemonWorker::getJobInfo(const QString& job) {
  QVariantMap result;
  LastoreJobInterface job_interface(kLastoreJobService,
                                    job,
                                    QDBusConnection::systemBus(),
                                    this);
  if (job_interface.isValid()) {
    result.insert("id", job_interface.id());
    result.insert("name", job_interface.name());
    result.insert("status", job_interface.status());
    result.insert("type", job_interface.type());
    result.insert("speed", job_interface.speed());
    result.insert("progress", job_interface.progress());
    result.insert("description", job_interface.description());
    result.insert("packages", job_interface.packages());
    result.insert("cancelable", job_interface.cancelable());
    emit this->getJobInfoReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
            { kResultName, job },
            { kResultValue, result },
          }
        },
    });
  } else {
    emit this->getJobInfoReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, job_interface.lastError().name() },
        { kResultErrMsg, job_interface.lastError().message() },
        { kResult, QVariantMap() },
    });
  }
}

void StoreDaemonWorker::openApp(const QString& app_name) {
  const QString desktop_file = manager_->PackageDesktopPath(app_name);
  if (!ExecuteDesktopFile(desktop_file)) {
    qWarning() << Q_FUNC_INFO << "failed to launch:" << app_name;
  }
}

}  // namespace dstore
