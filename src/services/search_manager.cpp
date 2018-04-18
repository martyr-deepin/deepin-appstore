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

SearchManager::SearchManager(QObject* parent)
    : QObject(parent),
      record_list_() {
  this->setObjectName("SearchManager");
}

SearchManager::~SearchManager() {

}

void SearchManager::searchApp(const QString& keyword) {
  qDebug() << Q_FUNC_INFO << keyword;
  AppSearchRecordList result;
  for (const AppSearchRecord& app : record_list_) {
    if (app.name.contains(keyword, Qt::CaseInsensitive) ||
        app.local_name.contains(keyword, Qt::CaseInsensitive) ||
        app.slogan.contains(keyword, Qt::CaseInsensitive) ||
        app.description.contains(keyword, Qt::CaseInsensitive)) {
      result.append(app);
    }
  }

  qDebug() << Q_FUNC_INFO << "search app result: " << result.size();
  emit this->searchAppResult(result);
}

void SearchManager::updateAppList(const AppSearchRecordList& record_list) {
  qDebug() << Q_FUNC_INFO << record_list.size();
  record_list_ = record_list;
}

}  // namespace dstore