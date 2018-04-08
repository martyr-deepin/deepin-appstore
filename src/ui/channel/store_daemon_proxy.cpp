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

void StoreDaemonProxy::setMirrorSource(const QString& src) {
  store_updater_iface_->SetMirrorSource(src);
}

void StoreDaemonProxy::setAutoDownloadUpdates(bool update) {
  store_updater_iface_->SetAutoDownloadUpdates(update);
}

void StoreDaemonProxy::setAutoCheckUpdates(bool check) {
  store_updater_iface_->SetAutoCheckUpdates(check);
}

QVariantList StoreDaemonProxy::listMirrorSources(const QString& opt) {
  QVariantList result;
  auto reply = store_updater_iface_->ListMirrorSources(opt);
  return result;
}

}  // namespace dstore