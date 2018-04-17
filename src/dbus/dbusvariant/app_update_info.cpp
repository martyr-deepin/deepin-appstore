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

#include "dbus/dbusvariant/app_update_info.h"

AppUpdateInfo::AppUpdateInfo() {
}

AppUpdateInfo::~AppUpdateInfo() {

}

// static
void AppUpdateInfo::registerMetaType() {
  qRegisterMetaType<AppUpdateInfo>("AppUpdateInfo");
  qDBusRegisterMetaType<AppUpdateInfo>();
  qRegisterMetaType<AppUpdateInfoList>("AppUpdateInfoList");
  qDBusRegisterMetaType<AppUpdateInfoList>();
}

QDebug operator<<(QDebug debug, const AppUpdateInfo& info) {
  debug << info.id
        << info.name
        << info.icon
        << info.current_version
        << info.last_version;
  return debug;
}

QDBusArgument& operator<<(QDBusArgument& argument,
                          const AppUpdateInfo& info) {
  argument.beginStructure();
  argument << info.id
           << info.name
           << info.icon
           << info.current_version
           << info.last_version;
  argument.endStructure();
  return argument;
}

QDataStream& operator<<(QDataStream& stream, const AppUpdateInfo& info) {
  stream << info.id
         << info.name
         << info.icon
         << info.current_version
         << info.last_version;
  return stream;
}

const QDBusArgument& operator>>(const QDBusArgument& argument,
                                AppUpdateInfo& info) {
  argument.beginStructure();
  argument >> info.id
           >> info.name
           >> info.icon
           >> info.current_version
           >> info.last_version;
  argument.endStructure();
  return argument;
}

const QDataStream& operator>>(QDataStream& stream, AppUpdateInfo& info) {
  stream >> info.id
         >> info.name
         >> info.icon
         >> info.current_version
         >> info.last_version;
  return stream;
}

const QVariantMap AppUpdateInfo::toVariantMap() const {
  const QVariantMap result {
      { "id", this->id },
      { "name", this->name },
      { "icon", this->icon },
      { "currentVersion", this->current_version },
      { "lastVersion", this->last_version },
  };
  return result;
}

const QVariantList AppUpdateInfoListToVariant(const AppUpdateInfoList& list) {
  QVariantList result;
  for (const AppUpdateInfo& info : list) {
    result.append(info.toVariantMap());
  }
  return result;
}
