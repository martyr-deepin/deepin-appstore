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
#include <QThread>

#include "base/launcher.h"
#include "dbus/dbus_consts.h"
#include "dbus/lastore_job_interface.h"
#include "dbus/lastore_manager_interface.h"
#include "dbus/lastore_updater_interface.h"

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
          this)),
      worker_thread_(new QThread(this)),
      worker_(new StoreDaemonWorker()) {

  this->setObjectName("StoreDaemonProxy");
  AppUpdateInfo::registerMetaType();
  LocaleMirrorSource::registerMetaType();

  this->initConnections();

  worker_thread_->start();
  worker_->moveToThread(worker_thread_);
}

StoreDaemonProxy::~StoreDaemonProxy() {
  worker_thread_->quit();
  worker_thread_->wait(3);
}

void StoreDaemonProxy::initConnections() {
  connect(worker_thread_, &QThread::finished,
          worker_, &StoreDaemonWorker::deleteLater);

  connect(worker_, &StoreDaemonWorker::isDbusConnectedReply,
          this, &StoreDaemonProxy::isDbusConnectedReply);

  connect(worker_, &StoreDaemonWorker::cleanArchivesReply,
          this, &StoreDaemonProxy::cleanArchivesReply);
  connect(worker_, &StoreDaemonWorker::cleanJobReply,
          this, &StoreDaemonProxy::cleanJobReply);
  connect(worker_, &StoreDaemonWorker::pauseJobReply,
          this, &StoreDaemonProxy::pauseJobReply);
  connect(worker_, &StoreDaemonWorker::startJobReply,
          this, &StoreDaemonProxy::startJobReply);
  connect(worker_, &StoreDaemonWorker::installPackageReply,
          this, &StoreDaemonProxy::installPackageReply);
  connect(worker_, &StoreDaemonWorker::packageExistsReply,
          this, &StoreDaemonProxy::packageExistsReply);
  connect(worker_, &StoreDaemonWorker::packageInstallableReply,
          this, &StoreDaemonProxy::packageInstallableReply);
  connect(worker_, &StoreDaemonWorker::packageDownloadSizeReply,
          this, &StoreDaemonProxy::packageDownloadSizeReply);
  connect(worker_, &StoreDaemonWorker::updatePackageReply,
          this, &StoreDaemonProxy::updatePackageReply);
  connect(worker_, &StoreDaemonWorker::removePackageReply,
          this, &StoreDaemonProxy::removePackageReply);
  connect(worker_, &StoreDaemonWorker::upgradableAppsReply,
          this, &StoreDaemonProxy::upgradableAppsReply);

  connect(worker_, &StoreDaemonWorker::applicationUpdateInfosReply,
          this, &StoreDaemonProxy::applicationUpdateInfosReply);

  connect(worker_, &StoreDaemonWorker::jobListReply,
          this, &StoreDaemonProxy::jobListReply);
  connect(worker_, &StoreDaemonWorker::getJobInfoReply,
          this, &StoreDaemonProxy::getJobInfoReply);
}

void StoreDaemonProxy::isDBusConnected() {
  emit worker_->isDbusConnectedRequest();
}

void StoreDaemonProxy::cleanArchives() {
  worker_->cleanArchivesRequest();
}

void StoreDaemonProxy::cleanJob(const QString& job) {
  emit worker_->cleanJobRequest(job);
}

void StoreDaemonProxy::installPackage(const QString& app_name) {
  emit worker_->installPackageRequest(app_name);
}

void StoreDaemonProxy::packageExists(const QString& app_name) {
  emit worker_->packageExistsRequest(app_name);
}

void StoreDaemonProxy::packageInstallable(const QString& app_name) {
  emit worker_->packageInstallableRequest(app_name);
}

void StoreDaemonProxy::packageDownloadSize(const QString& app_name) {
  emit worker_->packageDownloadSizeRequest(app_name);
}

void StoreDaemonProxy::pauseJob(const QString& job) {
  emit worker_->pauseJobRequest(job);
}

void StoreDaemonProxy::startJob(const QString& job) {
  emit worker_->startJobRequest(job);
}

void StoreDaemonProxy::updatePackage(const QString& app_name) {
  emit worker_->updatePackageRequest(app_name);
}

void StoreDaemonProxy::removePackage(const QString& app_name) {
  emit worker_->removePackageRequest(app_name);
}

void StoreDaemonProxy::upgradableApps() {
  emit worker_->upgradableAppsRequest();
}

void StoreDaemonProxy::applicationUpdateInfos(const QString& language) {
  emit worker_->applicationUpdateInfosRequest(language);
}

void StoreDaemonProxy::jobList() {
  emit worker_->jobListRequest();
}

void StoreDaemonProxy::getJobInfo(const QString& job) {
  emit worker_->getJobInfoRequest(job);
}

void StoreDaemonProxy::openApp(const QString& app_name) {
  emit worker_->openAppRequest(app_name);
}

}  // namespace dstore
