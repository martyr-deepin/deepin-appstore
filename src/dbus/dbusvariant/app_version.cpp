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

#include "dbus/dbusvariant/app_version.h"

#include <QtDBus/QtDBus>

AppVersion::AppVersion() {

}

AppVersion::~AppVersion() {

}

void AppVersion::registerMetaType() {
  qRegisterMetaType<AppVersion>("AppVersion");
  qDBusRegisterMetaType<AppVersion>();
  qRegisterMetaType<AppVersionList>("AppVersionList");
  qDBusRegisterMetaType<AppVersionList>();
}

QDebug operator<<(QDebug debug, const AppVersion& info) {
  debug << info.pkg_name
        << info.installed_version
        << info.remote_version
        << info.upgradable;
  return debug;
}

QDBusArgument& operator<<(QDBusArgument& argument,
                          const AppVersion& info) {
  argument.beginStructure();
  argument << info.pkg_name
           << info.installed_version
           << info.remote_version
           << info.upgradable;
  argument.endStructure();
  return argument;
}

QDataStream& operator<<(QDataStream& stream, const AppVersion& info) {
  stream << info.pkg_name
         << info.installed_version
         << info.remote_version
         << info.upgradable;
  return stream;
}

const QDBusArgument& operator>>(const QDBusArgument& argument,
                                AppVersion& info) {
  argument.beginStructure();
  argument >> info.pkg_name
           >> info.installed_version
           >> info.remote_version
           >> info.upgradable;
  argument.endStructure();
  return argument;
}

const QDataStream& operator>>(QDataStream& stream, AppVersion& info) {
  stream >> info.pkg_name
         >> info.installed_version
         >> info.remote_version
         >> info.upgradable;
  return stream;
}
