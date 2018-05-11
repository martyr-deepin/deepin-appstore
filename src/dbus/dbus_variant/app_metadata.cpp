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

#include "dbus/dbus_variant/app_metadata.h"

#include <QtDBus/QtDBus>

AppMetadata::AppMetadata() {

}

AppMetadata::~AppMetadata() {

}

void AppMetadata::registerMetaType() {
  qRegisterMetaType<AppMetadata>("AppMetadata");
  qDBusRegisterMetaType<AppMetadata>();
}

QDebug operator<<(QDebug debug, const AppMetadata& info) {
  debug << info.name;
  return debug;
}

QDBusArgument& operator<<(QDBusArgument& argument, const AppMetadata& info) {
  argument << info.name;
  return argument;
}

QDataStream& operator<<(QDataStream& stream, const AppMetadata& info) {
  stream << info.name;
  return stream;
}

const QDBusArgument& operator>>(const QDBusArgument& argument,
                                AppMetadata& info) {
  argument >> info.name;
  return argument;
}

const QDataStream& operator>>(QDataStream& stream, AppMetadata& info) {
  stream >> info.name;
  return stream;
}
