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

#ifndef DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_APP_UPDATE_INFO_H
#define DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_APP_UPDATE_INFO_H

#include <QString>

struct AppUpdateInfo {
 public:
  AppUpdateInfo();
  ~AppUpdateInfo();

  static void registerMetaType();

  inline bool operator==(const AppUpdateInfo& other) const {
    return this.id == other.id;
  }

  friend QDebug operator<<(QDebug debug, const AppUpdateInfo& info);
  friend QDBusArgument& operator<<(QDBusArgument& argument,
                                   const AppUpdateInfo& info);
  friend QDataStream& operator<<(QDataStream& stream,
                                 const AppUpdateInfo& info);
  friend const QDBusArgument& operator>>(const QDBusArgument & argument,
                                         AppUpdateInfo& info);
  friend const QDataStream& operator>>(QDataStream& stream, AppUpdateInfo& info);

  QString id;
  QString name;
  QString icon;
  QString current_version;
  QString last_version;
  QString changelog;
};

typedef QList<AppUpdateInfo> AppUpdateInfoList;
Q_DECLARE_METATYPE(AppUpdateInfo)
Q_DECLARE_METATYPE(AppUpdateInfoList)

#endif  // DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_APP_UPDATE_INFO_H
