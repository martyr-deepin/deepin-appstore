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

#include "services/dbus_manager.h"

#include <QCommandLineParser>
#include <QDebug>
#include <QtDBus>

#include "dbus/app_store_dbus_adapter.h"
#include "dbus/app_store_dbus_interface.h"
#include "dbus/dbus_consts.h"

namespace dstore {

DBusManager::DBusManager(QObject* parent) : QObject(parent) {

}

DBusManager::~DBusManager() {

}

bool DBusManager::parseArguments() {
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(QCommandLineOption(
      "dbus", "enable daemon mode"
  ));
  parser.parse(qApp->arguments());

  // Register dbus service.
  QDBusConnection session_bus = QDBusConnection::sessionBus();

  AppStoreDBusAdapter* adapter = new AppStoreDBusAdapter(this);
  Q_UNUSED(adapter);

  if (!session_bus.registerService(kAppStoreDbusService) ||
      !session_bus.registerObject(kAppStoreDbusPath, this)) {
    qWarning() << Q_FUNC_INFO
               << "Failed to register dbus service"
               << session_bus.lastError();

    // Failed to register dbus service.
    // Open app with dbus interface.
    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty()) {
      AppStoreDBusInterface* interface = new AppStoreDBusInterface(
          kAppStoreDbusService,
          kAppStoreDbusPath,
          session_bus,
          this
      );

      if (interface->isValid()) {
        // Only pass the first positional argument.
        interface->ShowAppDetail(args.first());
        return true;
      } else {
        app_name_ = args.first();
        return false;
      }
    } else {
      return true;
    }
  } else {
    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty()) {
      app_name_ = args.first();
    }
  }

  return false;
}

void DBusManager::Raise() {
  emit this->raiseRequested();
}

void DBusManager::ShowDetail(const QString& app_name) {
  emit this->showDetailRequested(app_name);
}

}  // namespace dstore