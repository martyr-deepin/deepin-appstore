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

#ifndef DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_APP_METADATA_H
#define DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_APP_METADATA_H

#include <QDBusArgument>
#include <QDebug>
#include <QList>
#include <QString>
#include <QVariant>

struct AppMetadata {
 public:
  AppMetadata();
  ~AppMetadata();

  static void registerMetaType();

  inline bool operator==(const AppMetadata& other) const {
    return this->name == other.name;
  }

  friend QDebug operator<<(QDebug debug, const AppMetadata& info);
  friend QDBusArgument& operator<<(QDBusArgument& argument,
                                   const AppMetadata& info);
  friend QDataStream& operator<<(QDataStream& stream,
                                 const AppMetadata& info);
  friend const QDBusArgument& operator>>(const QDBusArgument& argument,
                                         AppMetadata& info);
  friend const QDataStream& operator>>(QDataStream& stream,
                                       AppMetadata& info);

  QString name;
};

Q_DECLARE_METATYPE(AppMetadata);

#endif  // DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_APP_METADATA_H
