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

#include "services/search_manager.h"

#include <QDebug>

namespace dstore {

namespace {

const int kMaxSearchResult = 10;

}  // namespace

SearchManager::SearchManager(QObject* parent)
    : QObject(parent),
      record_list_() {
  this->setObjectName("SearchManager");
}

SearchManager::~SearchManager() {

}

void SearchManager::searchApp(const QString& keyword, bool entered) {
  AppSearchRecordList result;
  for (const AppSearchRecord& app : record_list_) {
    if (result.length() >= kMaxSearchResult) {
      break;
    }
    if (app.name.contains(keyword, Qt::CaseInsensitive) ||
        app.local_name.contains(keyword, Qt::CaseInsensitive) ||
        app.slogan.contains(keyword, Qt::CaseInsensitive) ||
        app.description.contains(keyword, Qt::CaseInsensitive)) {
      result.append(app);
    }
  }

  emit this->searchAppResult(keyword, entered, result);
}

void SearchManager::updateAppList(const AppSearchRecordList& record_list) {
  record_list_ = record_list;
}

}  // namespace dstore