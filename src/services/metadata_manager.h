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

#ifndef DEEPIN_APPSTORE_SERVICES_METADATA_MANAGER_H
#define DEEPIN_APPSTORE_SERVICES_METADATA_MANAGER_H

#include <QObject>
class QThread;

namespace dstore {

class MetadataCacheWorker;

// MetadataManager manages app metadata cache and app icon cache.
// Itself works in foreground thread, any http requests are sent in background.
class MetadataManager : public QObject {
  Q_OBJECT
 public:
  explicit MetadataManager(QObject* parent = nullptr);
  ~MetadataManager() override;

 private:
  void initConnections();

  MetadataCacheWorker* cache_worker_ = nullptr;
  QThread* cache_thread_ = nullptr;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_SERVICES_METADATA_MANAGER_H
