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

#include "dbus/dbus_variant/installed_app_timestamp.h"

#include <QtDBus/QtDBus>

InstalledAppTimestamp::InstalledAppTimestamp() {

}

InstalledAppTimestamp::~InstalledAppTimestamp() {

}

void InstalledAppTimestamp::registerMetaType() {
  qRegisterMetaType<InstalledAppTimestamp>("InstalledAppTimestamp");
  qDBusRegisterMetaType<InstalledAppTimestamp>();
  qRegisterMetaType<InstalledAppTimestampList>("InstalledAppTimestampList");
  qDBusRegisterMetaType<InstalledAppTimestampList>();
}

QDebug operator<<(QDebug debug, const InstalledAppTimestamp& info) {
  debug << info.pkg_name
        << info.timestamp;
  return debug;
}

QDBusArgument& operator<<(QDBusArgument& argument,
                          const InstalledAppTimestamp& info) {
  argument.beginStructure();
  argument << info.pkg_name
           << info.timestamp;
  argument.endStructure();
  return argument;
}

QDataStream& operator<<(QDataStream& stream, const InstalledAppTimestamp& info) {
  stream << info.pkg_name
         << info.timestamp;
  return stream;
}

const QDBusArgument& operator>>(const QDBusArgument& argument,
                                InstalledAppTimestamp& info) {
  argument.beginStructure();
  argument >> info.pkg_name
           >> info.timestamp;
  argument.endStructure();
  return argument;
}

const QDataStream& operator>>(QDataStream& stream, InstalledAppTimestamp& info) {
  stream >> info.pkg_name
         >> info.timestamp;
  return stream;
}
