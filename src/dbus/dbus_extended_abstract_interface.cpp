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

#include "dbus/dbus_extended_abstract_interface.h"

#include <QDebug>
#include <QtDBus/QtDBus>

namespace {

const char kPropIface[] = "org.freedesktop.DBus.Properties";
const char kPropName[] = "PropertiesChanged";
const char kPropType[] = "sa{sv}as";

}  // namespace

DbusExtendedAbstractInterface::DbusExtendedAbstractInterface(
    const QString& service,
    const QString& path,
    const char* interface,
    const QDBusConnection& connection,
    QObject* parent)
    : QDBusAbstractInterface(service, path, interface, connection, parent) {
  this->connection().connect(this->service(), this->path(), kPropIface,
                             kPropName, kPropType, this,
                             SLOT(propertyChanged(QDBusMessage)));
}


DbusExtendedAbstractInterface::~DbusExtendedAbstractInterface() {
  this->connection().disconnect(this->service(), this->path(), kPropIface,
                                kPropName, kPropType, this,
                                SLOT(propertyChanged(QDBusMessage)));
}

void DbusExtendedAbstractInterface::propertyChanged(const QDBusMessage& msg) {
  const QList<QVariant> arguments = msg.arguments();
  if (arguments.count() != 3) {
    return;
  }
  const QString interface_name = arguments.at(0).toString();
  if (interface_name != this->interface()) {
    return;
  }


  const QVariantMap changed_props = qdbus_cast<QVariantMap>(
      arguments.at(1).value<QDBusArgument>());
  for (const QString& prop : changed_props.keys()) {
    const QMetaObject* self = this->metaObject();
    for (int i = self->propertyOffset(); i < self->propertyCount(); ++i) {
      const QMetaProperty p = self->property(i);
      if (p.name() == prop) {
        emit p.notifySignal().invoke(this);
      }
    }
  }
}
