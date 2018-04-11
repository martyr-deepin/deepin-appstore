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

#include "dbus/dbus_consts.h"
#include "dbus/lastore_manager_interface.h"
#include "dbus/lastore_updater_interface.h"

namespace dstore {

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

QString StoreDaemonProxy::cleanArchives() {
  const QDBusObjectPath path = manager_->CleanArchives();
  return path.path();
}

void StoreDaemonProxy::cleanJob(const QString& job) {
  manager_->CleanJob(job);
}

QString StoreDaemonProxy::distUpgrade() {
  const QDBusObjectPath path = manager_->DistUpgrade();
  return path.path();
}

QString StoreDaemonProxy::installPackage(const QString& job,
                                         const QString& package) {
  const QDBusObjectPath path = manager_->InstallPackage(job, package);
  return path.path();
}

QString StoreDaemonProxy::packageDesktopPath(const QString& package) {
  return manager_->PackageDesktopPath(package);
}

bool StoreDaemonProxy::packageExists(const QString& package) {
  return manager_->PackageExists(package);
}

bool StoreDaemonProxy::packageInstallable(const QString& package) {
  return manager_->PackageInstallable(package);
}

qlonglong StoreDaemonProxy::packagesDownloadSize(const QStringList& packages) {
  return manager_->PackagesDownloadSize(packages);
}

void StoreDaemonProxy::pauseJob(const QString& job) {
  manager_->PauseJob(job);
}

QString StoreDaemonProxy::prepareDistUpgrade() {
  const QDBusObjectPath path = manager_->PrepareDistUpgrade();
  return path.path();
}

void StoreDaemonProxy::recordLocaleInfo(const QString& language) {
  manager_->RecordLocaleInfo(language);
}

void StoreDaemonProxy::startJob(const QString& job) {
  manager_->StartJob(job);
}

QString StoreDaemonProxy::updatePackage(const QString& job,
                                        const QString& packages) {
  const QDBusObjectPath path = manager_->UpdatePackage(job, packages);
  return path.path();
}

QString StoreDaemonProxy::updateSource() {
  const QDBusObjectPath path = manager_->UpdateSource();
  return path.path();
}

QString StoreDaemonProxy::removePackage(const QString& job,
                                        const QString& packages) {
  const QDBusObjectPath path = manager_->RemovePackage(job, packages);
  return path.path();
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

QStringList StoreDaemonProxy::jobList() {
  auto list = manager_->jobList();
  QStringList result;
  for (const QDBusObjectPath& path : list ) {
    result.append(path.path());
  }
  return result;
}

QStringList StoreDaemonProxy::systemArchitectures() {
  return manager_->systemArchitectures();
}

bool StoreDaemonProxy::systemOnChanging() {
  return manager_->systemOnChanging();
}

QStringList StoreDaemonProxy::upgradableApps() {
  return manager_->upgradableApps();
}

AppUpdateInfoList StoreDaemonProxy::applicationUpdateInfos(
    const QString& language) {
  return updater_->ApplicationUpdateInfos(language);
}

LocaleMirrorSourceList StoreDaemonProxy::listMirrorSources(
    const QString& language) {
  return updater_->ListMirrorSources(language);
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

QString StoreDaemonProxy::mirrorSource() {
  return updater_->mirrorSource();
}

QStringList StoreDaemonProxy::updatableApps() {
  return updater_->updatableApps();
}

QStringList StoreDaemonProxy::updatablePackages() {
  return updater_->updatablePackages();
}

}  // namespace dstore