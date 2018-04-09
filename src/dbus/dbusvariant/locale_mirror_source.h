/*
 * Copyright (C) 2017 ~ $year Deepin Technology Co., Ltd.
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

#ifndef DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_LOCALE_MIRROR_SOURCE_H
#define DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_LOCALE_MIRROR_SOURCE_H

#include <QString>
#include <QDebug>
#include <QtDBus>

struct LocaleMirrorSource {
 public:
  LocaleMirrorSource();
  ~LocaleMirrorSource();

  static void registerMetaType();

  inline bool operator==(const LocaleMirrorSource& other) const {
    return this->id == other.id;
  }

  friend QDebug operator<<(QDebug debug, const LocaleMirrorSource& src);
  friend QDBusArgument& operator<<(QDBusArgument& argument,
                                   const LocaleMirrorSource& src);
  friend QDataStream& operator<<(QDataStream& stream,
                                 const LocaleMirrorSource& src);
  friend const QDBusArgument& operator>>(const QDBusArgument & argument,
                                         LocaleMirrorSource& src);
  friend const QDataStream& operator>>(QDataStream& stream,
                                       LocaleMirrorSource& src);

  QString id;
  QString url;
  QString name;
};

typedef QList<LocaleMirrorSource> LocaleMirrorSourceList;
Q_DECLARE_METATYPE(LocaleMirrorSource)
Q_DECLARE_METATYPE(LocaleMirrorSourceList)

#endif  // DEEPIN_APPSTORE_DBUS_DBUS_VARIANT_LOCALE_MIRROR_SOURCE_H
