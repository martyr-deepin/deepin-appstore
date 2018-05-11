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

#include "services/backend/metadata_cache_worker.h"

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "base/file_util.h"

namespace dstore {

MetadataCacheWorker::MetadataCacheWorker(QObject* parent)
    : QObject(parent),
      network_manager_(new QNetworkAccessManager(this)) {
  this->setObjectName("MetadataCacheWorker");
}

MetadataCacheWorker::~MetadataCacheWorker() {

}

bool MetadataCacheWorker::downloadIcon(const QString& url,
                                       const QString& filepath) {
  Q_UNUSED(url);
  Q_UNUSED(filepath);

  QEventLoop loop;
  QNetworkRequest request;
  request.setUrl(QUrl(url));
  connect(network_manager_, &QNetworkAccessManager::finished,
          &loop, &QEventLoop::quit);
  QNetworkReply* reply = network_manager_->get(request);

  loop.exec();

  const QByteArray data = reply->readAll();

  return WriteRawFile(filepath, data);
}

bool MetadataCacheWorker::downloadAppList(const QString& url,
                                          const QString& filepath) {
  Q_UNUSED(url);
  Q_UNUSED(filepath);
  return false;
}

}  // namespace dstore