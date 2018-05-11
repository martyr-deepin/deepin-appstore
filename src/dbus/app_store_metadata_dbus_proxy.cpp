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

#include "dbus/app_store_metadata_dbus_proxy.h"

#include "services/apt_util_worker.h"

namespace dstore {

AppStoreMetadataDbusProxy::AppStoreMetadataDbusProxy(QObject* parent)
    : QObject(parent) {
  this->setObjectName("AppStoreMetadataDbusProxy");
}

AppStoreMetadataDbusProxy::~AppStoreMetadataDbusProxy() {

}

QString AppStoreMetadataDbusProxy::GetAppIcon(const QString& app_name) {
  Q_UNUSED(app_name);
  return QString();
}

AppMetadata AppStoreMetadataDbusProxy::GetAppMetadata(const QString& app_name) {
  Q_UNUSED(app_name);
  return AppMetadata();
}

void AppStoreMetadataDbusProxy::OpenApp(const QString& app_name) {
  dstore::OpenApp(app_name);
}

}  // namespace dstore