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

#include "ui/channel/store_daemon_proxy.h"

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
const char kResultValue[] = "value";

}  // namespace

StoreDaemonProxy::StoreDaemonProxy(QObject* parent)
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
  this->setObjectName("StoreDaemonProxy");
  AppUpdateInfo::registerMetaType();
  LocaleMirrorSource::registerMetaType();
}

StoreDaemonProxy::~StoreDaemonProxy() {

}

bool StoreDaemonProxy::isDBusConnected() const {
  return (manager_->isValid() && updater_->isValid());
}

const QVariantMap StoreDaemonProxy::cleanArchives() {
  const QDBusPendingReply<QDBusObjectPath> reply = manager_->CleanArchives();
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, "" },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, reply.value().path() },
    };
  }
}

const QVariantMap StoreDaemonProxy::cleanJob(const QString& job) {
  const QDBusPendingReply<> reply = manager_->CleanJob(job);
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, QVariant() },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, QVariant() },
    };
  }
}

const QString StoreDaemonProxy::distUpgrade() {
  const QDBusObjectPath path = manager_->DistUpgrade();
  return path.path();
}

const QVariantMap StoreDaemonProxy::installPackage(const QString& app_name) {
  // NOTE(Shaohua): package name is also set as job_name so that `name`
  // property in JobInfo is referred to package_name.
  const QDBusPendingReply<QDBusObjectPath> reply =
      manager_->InstallPackage(app_name, app_name);
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, "" },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, reply.value().path() },
    };
  }
}

const QString StoreDaemonProxy::packageDesktopPath(const QString& app_name) {
  return manager_->PackageDesktopPath(app_name);
}

const QVariantMap StoreDaemonProxy::packageExists(const QString& app_name) {
  const QDBusPendingReply<bool> reply = manager_->PackageExists(app_name);
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, false },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, reply.value() },
    };
  }
}

const QVariantMap StoreDaemonProxy::packageInstallable(const QString& app_name) {
  const QDBusPendingReply<bool> reply = manager_->PackageInstallable(app_name);
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, false },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, reply.value() },
    };
  }
}

const QVariantMap StoreDaemonProxy::packageDownloadSize(
    const QString& app_name) {
  const QDBusPendingReply<qlonglong> reply =
      manager_->PackagesDownloadSize({app_name});
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, 0 },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, reply.value() },
    };
  }
}

const QVariantMap StoreDaemonProxy::pauseJob(const QString& job) {
  const QDBusPendingReply<> reply = manager_->PauseJob(job);
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, QVariant() },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, QVariant() },
    };
  }
}

const QString StoreDaemonProxy::prepareDistUpgrade() {
  const QDBusObjectPath path = manager_->PrepareDistUpgrade();
  return path.path();
}

void StoreDaemonProxy::recordLocaleInfo(const QString& language) {
  manager_->RecordLocaleInfo(language);
}

const QVariantMap StoreDaemonProxy::startJob(const QString& job) {
  const QDBusPendingReply<> reply = manager_->StartJob(job);
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, QVariant() },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, QVariant() },
    };
  }
}

const QVariantMap StoreDaemonProxy::updatePackage(const QString& app_name) {
  const QDBusPendingReply<QDBusObjectPath> reply =
      manager_->UpdatePackage(app_name, app_name);
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, "" },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, reply.value().path() },
    };
  }
}

const QString StoreDaemonProxy::updateSource() {
  const QDBusObjectPath path = manager_->UpdateSource();
  return path.path();
}

const QVariantMap StoreDaemonProxy::removePackage(const QString& app_name) {
  const QDBusPendingReply<QDBusObjectPath> reply =
      manager_->RemovePackage(app_name, app_name);
  if (reply.isError()) {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, reply.error().name() },
        { kResultErrMsg, reply.error().message() },
        { kResultValue, "" },
    };
  } else {
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, reply.value().path() },
    };
  }
}

void StoreDaemonProxy::setAutoClean(bool enabled) {
  manager_->SetAutoClean(enabled);
}

void StoreDaemonProxy::setRegion(const QString& region) {
  manager_->SetRegion(region);
}

bool StoreDaemonProxy::autoClean() {
  return manager_->autoClean();
}

const QVariantMap StoreDaemonProxy::jobList() {
  auto list = manager_->jobList();
  QStringList result;
  for (const QDBusObjectPath& path : list ) {
    result.append(path.path());
  }

  return QVariantMap {
      { kResultOk, true },
      { kResultErrName, "" },
      { kResultErrMsg, "" },
      { kResultValue, result },
  };
}

const QStringList StoreDaemonProxy::systemArchitectures() {
  return manager_->systemArchitectures();
}

bool StoreDaemonProxy::systemOnChanging() {
  return manager_->systemOnChanging();
}

const QVariantMap StoreDaemonProxy::upgradableApps() {
  const QStringList reply = manager_->upgradableApps();
  return QVariantMap {
      { kResultOk, true },
      { kResultErrName, "" },
      { kResultErrMsg, "" },
      { kResultValue, reply },
  };
}

const QVariantList StoreDaemonProxy::applicationUpdateInfos(
    const QString& language) {
  const AppUpdateInfoList list = updater_->ApplicationUpdateInfos(language);
  return AppUpdateInfoListToVariant(list);
}

const QVariantList StoreDaemonProxy::listMirrorSources(
    const QString& language) {
  const LocaleMirrorSourceList mirrors = updater_->ListMirrorSources(language);
  return LocaleMirrorSourceListToVariant(mirrors);
}

void StoreDaemonProxy::setAutoCheckUpdates(bool check) {
  updater_->SetAutoCheckUpdates(check);
}

void StoreDaemonProxy::setAutoDownloadUpdates(bool update) {
  updater_->SetAutoDownloadUpdates(update);
}

void StoreDaemonProxy::setMirrorSource(const QString& id) {
  updater_->SetMirrorSource(id);
}

bool StoreDaemonProxy::autoCheckUpdates() {
  return updater_->autoCheckUpdates();
}

bool StoreDaemonProxy::autoDownloadUpdates() {
  return updater_->autoDownloadUpdates();
}

const QString StoreDaemonProxy::mirrorSource() {
  return updater_->mirrorSource();
}

//QStringList StoreDaemonProxy::updatableApps() {
//  return updater_->updatableApps();
//}
//
//QStringList StoreDaemonProxy::updatablePackages() {
//  return updater_->updatablePackages();
//}

const QVariantMap StoreDaemonProxy::getJobInfo(const QString& job) {
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
    return QVariantMap {
        { kResultOk, true },
        { kResultErrName, "" },
        { kResultErrMsg, "" },
        { kResultValue, result },
    };
  } else {
    return QVariantMap {
        { kResultOk, false },
        { kResultErrName, job_interface.lastError().name() },
        { kResultErrMsg, job_interface.lastError().message() },
        { kResultValue, QVariantMap() },
    };
  }
}

void StoreDaemonProxy::openApp(const QString& app_name) {
  const QString desktop_file = manager_->PackageDesktopPath(app_name);
  ExecuteDesktopFile(desktop_file);
}

}  // namespace dstore
