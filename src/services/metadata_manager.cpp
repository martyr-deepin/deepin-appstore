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

#include "services/metadata_manager.h"

#include <QThread>

#include "services/backend/metadata_cache_worker.h"

namespace dstore {

MetadataManager::MetadataManager(QObject* parent)
    : QObject(parent),
      cache_worker_(new MetadataCacheWorker),
      cache_thread_(new QThread(this)) {
  this->setObjectName("MetadataManager");

  cache_thread_->start();

  cache_worker_->moveToThread(cache_thread_);

  this->initConnections();
}

MetadataManager::~MetadataManager() {
  cache_thread_->quit();
  cache_thread_->wait(3);
}

void MetadataManager::initConnections() {
  connect(cache_thread_, &QThread::finished,
          cache_worker_, &MetadataCacheWorker::deleteLater);
}

}  // namespace dstore