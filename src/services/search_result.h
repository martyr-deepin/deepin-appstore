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

#ifndef DEEPIN_APPSTORE_SERVICES_SEARCH_RESULT_H
#define DEEPIN_APPSTORE_SERVICES_SEARCH_RESULT_H

#include <QDebug>
#include <QList>
#include <QHash>
#include <QtCore/QMetaType>

namespace dstore {

// App entry used in search service.
struct SearchMeta {
  QString name;
  QString local_name;
  QString slogan;
  QString description;
  QStringList package_uris;

  // Package names used in deb format.
  QStringList debs;

  // Package names used in flatpak format.
  QStringList flatpaks;
};

bool operator==(const SearchMeta& a, const SearchMeta& b);

QDebug& operator<<(QDebug& debug, const SearchMeta& app);

void RegisterSearchMetaMetaType();

bool operator<(const SearchMeta& a, const SearchMeta& b);

typedef QList<SearchMeta> SearchMetaList;

typedef QHash<QString, SearchMeta> SearchMetaMap;

}  // namespace dstore

Q_DECLARE_METATYPE(dstore::SearchMeta);
Q_DECLARE_METATYPE(dstore::SearchMetaList);

#endif  // DEEPIN_APPSTORE_SERVICES_SEARCH_RESULT_H
