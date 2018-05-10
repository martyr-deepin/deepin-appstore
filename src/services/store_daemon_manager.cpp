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
  Q_UNUSED(app_name);
}

void StoreDaemonManager::packageExists(const QString& app_name) {
  Q_UNUSED(app_name);
}

void StoreDaemonManager::packageInstallable(const QString& app_name) {
  Q_UNUSED(app_name);
}

void StoreDaemonManager::packageDownloadSize(const QString& app_name) {
  Q_UNUSED(app_name);
}

void StoreDaemonManager::updatePackage(const QString& app_name) {
  Q_UNUSED(app_name);
}

void StoreDaemonManager::removePackage(const QString& app_name) {
  Q_UNUSED(app_name);
}

void StoreDaemonManager::jobList() {

}

void StoreDaemonManager::upgradableApps() {

}

void StoreDaemonManager::applicationUpdateInfos(const QString& language) {
  Q_UNUSED(language);
}

void StoreDaemonManager::getJobInfo(const QString& job) {
  Q_UNUSED(job);
}

}  // namespace dstore