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

#ifndef DEEPIN_APPSTORE_DBUS_DBUS_EXTENDED_ABSTRACT_INTERFACE_H
#define DEEPIN_APPSTORE_DBUS_DBUS_EXTENDED_ABSTRACT_INTERFACE_H

#include <QDBusAbstractInterface>

/**
 * Extend qt dbus abstract interface to implements PropertiesChanged()
 * Subclass needs to statement its own signal names.
 */
class DbusExtendedAbstractInterface : public QDBusAbstractInterface {
  Q_OBJECT
 public:
  DbusExtendedAbstractInterface(const QString& service,
                                const QString& path,
                                const char* interface,
                                const QDBusConnection& connection,
                                QObject* parent);

  ~DbusExtendedAbstractInterface();

 private slots:
  void propertyChanged(const QDBusMessage& msg);
};

#endif  // DEEPIN_APPSTORE_DBUS_DBUS_EXTENDED_ABSTRACT_INTERFACE_H
