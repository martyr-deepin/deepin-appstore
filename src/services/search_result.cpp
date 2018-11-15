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

#include "services/search_result.h"

#include <QtCore/QMetaType>

namespace dstore {

bool operator<(const SearchMeta& a, const SearchMeta& b) {
  return a.name < b.name;
}

bool operator==(const SearchMeta& a, const SearchMeta& b) {
return a.name == b.name;
}

QDebug& operator<<(QDebug& debug, const SearchMeta& app) {
  debug << "App {"
        << "name:" << app.name
        << ", local_name:" << app.local_name
        << ", slogan:" << app.slogan
        << ", description:" << app.description
        << ", packages:" << app.package_uris
        << ", debs:" << app.debs;

  return debug;
}

void RegisterSearchMetaMetaType() {
  qRegisterMetaType<SearchMeta>("SearchMeta");
  qRegisterMetaType<SearchMetaList>("SearchMetaList");
}

}  // namespace dstore
