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

StoreDaemonProxy::StoreDaemonProxy(QObject* parent) : QObject(parent) {
  // TODO(Shaohua): Handles dbus connection error.
  store_manager_iface_ = new LastoreManagerInterface(
      kLastoreManagerService,
      kLastoreManagerInterface,
      QDBusConnection::sessionBus(),
      this);
  store_updater_iface_ = new LastoreUpdaterInterface(
      kLastoreUpdaterService,
      kLastoreUpdaterInterface,
      QDBusConnection::sessionBus(),
      this);
  AppUpdateInfo::registerMetaType();
}

StoreDaemonProxy::~StoreDaemonProxy() {

}

QString StoreDaemonProxy::cleanArchives() {
  return QString();
}

void StoreDaemonProxy::cleanJob(const QString& job) {
  Q_UNUSED(job);
}

QString StoreDaemonProxy::distUpgrade() {
  return QString();
}

QString StoreDaemonProxy::installPackage(const QString& job,
                                         const QString& package) {
  Q_UNUSED(job);
  Q_UNUSED(package);
  return QString();
}

QString StoreDaemonProxy::packageDesktopPath(const QString& package) {
  Q_UNUSED(package);
  return QString();
}

bool StoreDaemonProxy::packageExists(const QString& package) {
  Q_UNUSED(package);
  return false;
}

bool StoreDaemonProxy::packageInstallable(const QString& package) {
  Q_UNUSED(package);
  return false;
}

qint64 StoreDaemonProxy::packagesDownloadSize(const QString& package) {
  Q_UNUSED(package);
  return 0;
}

void StoreDaemonProxy::pauseJob(const QString& job) {
  Q_UNUSED(job);
}

QString StoreDaemonProxy::prepareDistUpgrade() {
  return QString();
}

void StoreDaemonProxy::recordLocaleInfo() {

}

void StoreDaemonProxy::startJob(const QString& job) {
  Q_UNUSED(job);
}

QString StoreDaemonProxy::updatePackage(const QString& job,
                                        const QString& packages) {
  Q_UNUSED(job);
  Q_UNUSED(packages);
  return QString();
}

QString StoreDaemonProxy::updateSource() {
  return QString();
}

QString StoreDaemonProxy::removePackage(const QString& job,
                                        const QString& packages) {
  Q_UNUSED(job);
  Q_UNUSED(packages);
  return QString();
}

void StoreDaemonProxy::setAutoClean(bool enabled) {
  Q_UNUSED(enabled);
}

void StoreDaemonProxy::setRegion(const QString& region) {
  Q_UNUSED(region);
}

bool StoreDaemonProxy::autoClean() {
  return false;
}

QStringList StoreDaemonProxy::jobList() {
  return QStringList();
}

QStringList StoreDaemonProxy::systemArchitectures() {
  return QStringList();
}

bool StoreDaemonProxy::systemOnChanging() {
  return false;
}

QStringList StoreDaemonProxy::upgradableApps() {
  return QStringList();
}

AppUpdateInfoList StoreDaemonProxy::applicationUpdateInfos(
    const QString& language) {
  Q_UNUSED(language);
  return AppUpdateInfoList();
}

LocaleMirrorSourceList StoreDaemonProxy::listMirrorSources(
    const QString& language) {
  Q_UNUSED(language);
  return LocaleMirrorSourceList();
}

void StoreDaemonProxy::setAutoCheckUpdates(bool check) {
  Q_UNUSED(check);
}

void StoreDaemonProxy::setAutoDownloadUpdates(bool update) {
  Q_UNUSED(update);
}

void StoreDaemonProxy::setMirrorSource(const QString& id) {
  Q_UNUSED(id);
}

bool StoreDaemonProxy::autoCheckUpdates() {
  return false;
}

bool StoreDaemonProxy::autoDownloadUpdates() {
  return false;
}

QString StoreDaemonProxy::mirrorSource() {
  return QString();
}

QStringList StoreDaemonProxy::updatableApps() {
  return QStringList();
}

QStringList StoreDaemonProxy::updatablePackages() {
  return QStringList();
}

}  // namespace dstore