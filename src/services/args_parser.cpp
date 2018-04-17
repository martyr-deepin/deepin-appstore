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

#include "services/args_parser.h"

#include <QCommandLineParser>
#include <QDebug>
#include <QtDBus>

#include "dbus/app_store_dbus_adapter.h"
#include "dbus/app_store_dbus_interface.h"
#include "dbus/app_store_dbus_proxy.h"
#include "dbus/dbus_consts.h"

namespace dstore {

ArgsParser::ArgsParser(QObject* parent) : QObject(parent) {

}

ArgsParser::~ArgsParser() {

}

bool ArgsParser::parseArguments() {
  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOption(QCommandLineOption(
      "dbus", "enable daemon mode"
  ));
  parser.parse(qApp->arguments());

  // Register dbus service.
  QDBusConnection session_bus = QDBusConnection::sessionBus();
  AppStoreDbusProxy* proxy = new AppStoreDbusProxy(this);
  connect(proxy, &AppStoreDbusProxy::raiseRequested,
          this, &ArgsParser::raiseRequested);
  connect(proxy, &AppStoreDbusProxy::openAppRequested,
          this, &ArgsParser::openAppRequested);

  AppStoreDBusAdapter* adapter = new AppStoreDBusAdapter(proxy);
  Q_UNUSED(adapter);

  if (!session_bus.registerService(kAppStoreDbusService) ||
      !session_bus.registerObject(kAppStoreDbusPath, proxy)) {
    qWarning() << Q_FUNC_INFO << "Failed to register dbus service";

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
        interface->OpenApp(args.first());
        return true;
      } else {
        app_name_ = args.first();
        return false;
      }
    } else {
      return true;
    }
  } else {
    qDebug() << "Register dbus service successfully.";
    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty()) {
      app_name_ = args.first();
    }
  }

  return false;
}

void ArgsParser::openAppDelay() {
  if (!app_name_.isEmpty()) {
    emit this->openAppRequested(app_name_);
  }
}

}  // namespace dstore