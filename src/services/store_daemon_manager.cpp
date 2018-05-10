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
#include "dbus/dbusvariant/app_version.h"
#include "dbus/dbusvariant/installed_app_info.h"
#include "dbus/lastore_deb_interface.h"
#include "dbus/lastore_job_interface.h"
#include "services/apt_util_worker.h"

namespace dstore {

namespace {

const char kResultOk[] = "ok";
const char kResultErrName[] = "errorName";
const char kResultErrMsg[] = "errorMsg";
const char kResult[] = "result";
const char kResultName[] = "name";
const char kResultValue[] = "value";

}  // namespace

StoreDaemonManager::StoreDaemonManager(QObject* parent)
    : QObject(parent),
      apps_(),
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

  apt_worker_thread_->start();
  apt_worker_->moveToThread(apt_worker_thread_);

  this->initConnections();
}

StoreDaemonManager::~StoreDaemonManager() {
  apt_worker_thread_->quit();
  apt_worker_thread_->wait(3);
}

void StoreDaemonManager::initConnections() {
  connect(this, &StoreDaemonManager::cleanArchivesRequest,
          apt_worker_, &AptUtilWorker::cleanArchivesRequest);
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

  connect(this, &StoreDaemonManager::packageExistsRequest,
          this, &StoreDaemonManager::packageExists);
  connect(this, &StoreDaemonManager::packageInstallableRequest,
          this, &StoreDaemonManager::packageInstallable);

  connect(this, &StoreDaemonManager::getJobInfoRequest,
          this, &StoreDaemonManager::getJobInfo);
}

void StoreDaemonManager::updateAppList(const AppSearchRecordList& app_list) {
  apps_.clear();
  for (const AppSearchRecord& app : app_list) {
    apps_.insert(app.name, app);
  }
}

void StoreDaemonManager::isDBusConnected() {
  const bool state = deb_interface_->isValid();
  emit this->isDbusConnectedReply(state);
}

void StoreDaemonManager::cleanJob(const QString& job) {
  Q_UNUSED(job);
}

void StoreDaemonManager::pauseJob(const QString& job) {
  Q_UNUSED(job);
}

void StoreDaemonManager::startJob(const QString& job) {
  Q_UNUSED(job);
}

void StoreDaemonManager::installPackage(const QString& app_name) {
  deb_interface_->Install(app_name, app_name);
  // NOTE(Shaohua): package name is also set as job_name so that `name`
  // property in JobInfo is referred to package_name.
  const QDBusPendingReply<QDBusObjectPath> reply =
      deb_interface_->Install(app_name, app_name);
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

void StoreDaemonManager::packageExists(const QString& app_name) {
  qDebug() << Q_FUNC_INFO << app_name;
  const QDBusPendingReply<AppVersionList> reply =
      deb_interface_->QueryVersion({app_name});
  if (reply.isError()) {
    emit this->packageExistsReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariant() },
    });
  } else {
    const AppVersionList list = reply.value();
    if (list.size() != 1 || list.first().pkg_name != app_name) {
      emit this->packageExistsReply(QVariantMap {
          { kResultOk, false },
          { kResultErrName, app_name },
          { kResultErrMsg, "Package not found" },
          { kResult, QVariant() },
      });
    } else {
      const bool installed = (!list.first().installed_version.isEmpty());
      emit this->packageExistsReply(QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResult, QVariantMap {
          { kResultName, app_name },
          { kResultValue, installed },
          },
        }
      });
    }
  }
}

void StoreDaemonManager::packageInstallable(const QString& app_name) {
  qDebug() << Q_FUNC_INFO << app_name;
  const QDBusPendingReply<AppVersionList> reply =
      deb_interface_->QueryVersion({app_name});
  if (reply.isError()) {
    emit this->packageInstallableReply(QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResult, QVariant() },
    });
  } else {
    const AppVersionList list = reply.value();
    if (list.size() != 1 || list.first().pkg_name != app_name) {
      emit this->packageInstallableReply(QVariantMap {
          { kResultOk, false },
          { kResultErrName, app_name },
          { kResultErrMsg, "Package not found" },
          { kResult, QVariant() },
      });
    } else {
      const bool exists = (!list.first().remote_version.isEmpty());
      emit this->packageInstallableReply(QVariantMap {
          { kResultOk, true },
          { kResultErrName, "" },
          { kResultErrMsg, "" },
          { kResult, QVariantMap {
              { kResultName, app_name },
              { kResultValue, exists },
          },
          }
      });
    }
  }
}

void StoreDaemonManager::packageDownloadSize(const QString& app_name) {
  Q_UNUSED(app_name);
}

void StoreDaemonManager::updatePackage(const QString& app_name) {
  this->installPackage(app_name);
}

void StoreDaemonManager::removePackage(const QString& app_name) {
  const QDBusPendingReply<QDBusObjectPath> reply =
      deb_interface_->Remove(app_name, app_name);
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

void StoreDaemonManager::jobList() {

}

void StoreDaemonManager::upgradableApps() {

}

void StoreDaemonManager::applicationUpdateInfos(const QString& language) {
  Q_UNUSED(language);
}

void StoreDaemonManager::getJobInfo(const QString& job) {
  QVariantMap result;
  LastoreJobInterface job_interface(kLastoreJobService,
                                    job,
                                    QDBusConnection::sessionBus(),
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
    result.insert("downloadSize", job_interface.downloadSize());
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

}  // namespace dstore