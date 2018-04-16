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

#include "dbus/dbusvariant/locale_mirror_source.h"


LocaleMirrorSource::LocaleMirrorSource() {
}

LocaleMirrorSource::~LocaleMirrorSource() {

}

// static
void LocaleMirrorSource::registerMetaType() {
  qRegisterMetaType<LocaleMirrorSource>("LocaleMirrorSource");
  qDBusRegisterMetaType<LocaleMirrorSource>();
  qRegisterMetaType<LocaleMirrorSourceList>("LocaleMirrorSourceList");
  qDBusRegisterMetaType<LocaleMirrorSourceList>();
}

QDebug operator<<(QDebug debug, const LocaleMirrorSource& info) {
  debug << info.id
        << info.url
      << info.name;
  return debug;
}

QDBusArgument& operator<<(QDBusArgument& argument,
                          const LocaleMirrorSource& src) {
  argument.beginStructure();
  argument << src.id
           << src.url
           << src.name;
  argument.endStructure();
  return argument;
}

QDataStream& operator<<(QDataStream& stream, const LocaleMirrorSource& info) {
  stream << info.id
         << info.url
         << info.name;
  return stream;
}

const QDBusArgument& operator>>(const QDBusArgument& argument,
                                LocaleMirrorSource& src) {
  argument.beginStructure();
  argument >> src.id
           >> src.url
           >> src.name;
  argument.endStructure();
  return argument;
}

const QDataStream& operator>>(QDataStream& stream, LocaleMirrorSource& src) {
  stream >> src.id
         >> src.url
         >> src.name;
  return stream;
}

const QVariantMap LocaleMirrorSource::toVariantMap() const {
  const QVariantMap result {
      { "id", this->id },
      { "url", this->url },
      { "name", this->name }
  };
  return result;
}

const QVariantList LocaleMirrorSourceListToVariant(
    const LocaleMirrorSourceList& list) {
  QVariantList result;
  for (const LocaleMirrorSource& mirror : list) {
    result.append(mirror.toVariantMap());
  }
  return result;
}
