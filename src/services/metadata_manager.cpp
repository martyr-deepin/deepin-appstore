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

#include <QDebug>

#include "base/file_util.h"
#include "services/backend/metadata_cache_worker.h"

namespace dstore {

namespace {

QString GetCacheDir() {
  const char kAppCacheDir[] = ".cache/deepin/deepin-appstore-metadata";
  return QDir::home().absoluteFilePath(kAppCacheDir);
}

}  // namespace

MetadataManager::MetadataManager(QObject* parent)
    : QObject(parent),
      cache_worker_(new MetadataCacheWorker(this)),
      cache_dir_(GetCacheDir()) {
  this->setObjectName("MetadataManager");

  CreateParentDirs(cache_dir_.absolutePath());

  this->initConnections();
}

MetadataManager::~MetadataManager() {
}

void MetadataManager::initConnections() {
}

QString MetadataManager::getAppIcon(const QString& app_name) {
  const QString filepath = cache_dir_.absoluteFilePath(app_name);
  if (QFileInfo::exists(filepath)) {
    return filepath;
  }

  const QString url = "http://server-13:8000/images/vivaldi-stable-icon.svg";
  if (cache_worker_->downloadIcon(url, filepath)) {
    return filepath;
  }

  return QString();
}

}  // namespace dstore