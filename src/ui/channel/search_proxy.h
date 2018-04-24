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

#ifndef DEEPIN_APPSTORE_UI_CHANNEL_SEARCH_PROXY_H
#define DEEPIN_APPSTORE_UI_CHANNEL_SEARCH_PROXY_H

#include <QObject>

#include "services/search_result.h"

namespace dstore {

class SearchProxy : public QObject {
  Q_OBJECT
 public:
  explicit SearchProxy(QObject* parent = nullptr);
  ~SearchProxy() override;

 signals:
  void onAppListUpdated(const AppSearchRecordList& record_list);

  /**
   * Request to open app info page
   * @param name
   */
  void openApp(const QString& name);

  /**
   * Request to open app search result page
   * @param keyword search keyword
   * @param names
   */
  void openAppList(const QString& keyword, const QStringList& names);

 public slots:
  /**
   * Update app list used in search service.
   * @param apps Serialized application info
   */
  void updateAppList(const QString& apps);
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_CHANNEL_SEARCH_PROXY_H
