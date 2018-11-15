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

#ifndef DEEPIN_APPSTORE_SERVICES_SEARCH_MANAGER_H
#define DEEPIN_APPSTORE_SERVICES_SEARCH_MANAGER_H

#include <QObject>

#include "services/search_result.h"

namespace dstore {

/**
 * Provides search service in backend.
 */
class SearchManager : public QObject {
  Q_OBJECT
 public:
  explicit SearchManager(QObject* parent = nullptr);
  ~SearchManager() override;

 signals:
  /**
   * Emitted after searchApp() is successfully handled.
   * @param keyword
   * @param record_list matched apps, might be empty.
   */
  void searchAppResult(const QString& keyword,
                       const SearchMetaList& record_list);

  void completeSearchAppResult(const QString& keyword,
                               const SearchMetaList& record_list);

 public slots:
  void searchApp(const QString& keyword);

  void completeSearchApp(const QString& keyword);

  void updateAppList(const SearchMetaList& app_list);

 private:
  SearchMetaList app_list_;

  QStringList app_names_pinyin_;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_SERVICES_SEARCH_MANAGER_H
