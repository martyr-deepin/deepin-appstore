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

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDebug>

#include "dbus/dbus_consts.h"
#include "dbus/app_store_metadata_dbus_proxy.h"

int main(int argc, char** argv) {
  QCoreApplication app(argc, argv);

  dstore::AppStoreMetadataDbusProxy proxy;

  QDBusConnection conn = QDBusConnection::sessionBus();
  if (!conn.registerService(dstore::kAppStoreMetadataDbusService) ||
      !conn.registerObject(dstore::kAppStoreMetadataDbusPath,
       &proxy,
       QDBusConnection::ExportScriptableContents)) {
    qCritical() << "Failed to register dbus service";
    return 1;
  }

  return app.exec();
}
