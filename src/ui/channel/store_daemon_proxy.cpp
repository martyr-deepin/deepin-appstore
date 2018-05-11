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

#include "base/launcher.h"
#include "dbus/dbus_consts.h"
#include "dbus/lastore_job_interface.h"

namespace dstore {

StoreDaemonProxy::StoreDaemonProxy(QObject* parent)
    : QObject(parent),
      manager_thread_(new QThread(this)),
      manager_(new StoreDaemonManager()) {

  this->setObjectName("StoreDaemonProxy");

  RegisterAppSearchRecordMetaType();

  this->initConnections();

  manager_thread_->start();
  manager_->moveToThread(manager_thread_);
}

StoreDaemonProxy::~StoreDaemonProxy() {
  manager_thread_->quit();
  manager_thread_->wait(3);
}

void StoreDaemonProxy::initConnections() {
  connect(manager_thread_, &QThread::finished,
          manager_, &StoreDaemonManager::deleteLater);

  connect(manager_, &StoreDaemonManager::isDbusConnectedReply,
          this, &StoreDaemonProxy::isDbusConnectedReply);

  connect(manager_, &StoreDaemonManager::cleanJobReply,
          this, &StoreDaemonProxy::cleanJobReply);
  connect(manager_, &StoreDaemonManager::pauseJobReply,
          this, &StoreDaemonProxy::pauseJobReply);
  connect(manager_, &StoreDaemonManager::startJobReply,
          this, &StoreDaemonProxy::startJobReply);
  connect(manager_, &StoreDaemonManager::installPackageReply,
          this, &StoreDaemonProxy::installPackageReply);
  connect(manager_, &StoreDaemonManager::packageDownloadSizeReply,
          this, &StoreDaemonProxy::packageDownloadSizeReply);
  connect(manager_, &StoreDaemonManager::updatePackageReply,
          this, &StoreDaemonProxy::updatePackageReply);
  connect(manager_, &StoreDaemonManager::removePackageReply,
          this, &StoreDaemonProxy::removePackageReply);
  connect(manager_, &StoreDaemonManager::queryVersionsReply,
          this, &StoreDaemonProxy::queryVersionsReply);


  connect(manager_, &StoreDaemonManager::installedPackagesReply,
          this, &StoreDaemonProxy::installedPackagesReply);

  connect(manager_, &StoreDaemonManager::jobListReply,
          this, &StoreDaemonProxy::jobListReply);
  connect(manager_, &StoreDaemonManager::getJobInfoReply,
          this, &StoreDaemonProxy::getJobInfoReply);

  connect(this, &StoreDaemonProxy::updateAppList,
          manager_, &StoreDaemonManager::updateAppList);
}

void StoreDaemonProxy::isDBusConnected() {
  emit manager_->isDbusConnectedRequest();
}

void StoreDaemonProxy::cleanArchives() {
  emit manager_->cleanArchivesRequest();
}

void StoreDaemonProxy::cleanJob(const QString& job) {
  emit manager_->cleanJobRequest(job);
}

void StoreDaemonProxy::installPackage(const QString& app_name) {
  emit manager_->installPackageRequest(app_name);
}

void StoreDaemonProxy::installedPackages() {
  emit manager_->installedPackagesRequest();
}

void StoreDaemonProxy::packageDownloadSize(const QString& app_name) {
  emit manager_->packageDownloadSizeRequest(app_name);
}

void StoreDaemonProxy::pauseJob(const QString& job) {
  emit manager_->pauseJobRequest(job);
}

void StoreDaemonProxy::startJob(const QString& job) {
  emit manager_->startJobRequest(job);
}

void StoreDaemonProxy::updatePackage(const QString& app_name) {
  emit manager_->updatePackageRequest(app_name);
}

void StoreDaemonProxy::removePackage(const QString& app_name) {
  emit manager_->removePackageRequest(app_name);
}

void StoreDaemonProxy::queryVersions(const QString& task_id,
                                     const QStringList& apps) {
  emit manager_->queryVersionsRequest(task_id, apps);
}

void StoreDaemonProxy::jobList() {
  emit manager_->jobListRequest();
}

void StoreDaemonProxy::getJobInfo(const QString& job) {
  emit manager_->getJobInfoRequest(job);
}

void StoreDaemonProxy::openApp(const QString& app_name) {
  emit manager_->openAppRequest(app_name);
}

}  // namespace dstore
