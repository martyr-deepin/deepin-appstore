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

#include "ui/channel/image_viewer_proxy.h"

#include <QDebug>
#include <QPixmap>

#include "base/file_util.h"

namespace dstore {

ImageViewerProxy::ImageViewerProxy(QObject* parent) : QObject(parent) {
  this->setObjectName("ImageViewerProxy");
}

ImageViewerProxy::~ImageViewerProxy() {

}

void ImageViewerProxy::open(const QString& filepath) {
  emit this->openImageFileRequested(filepath);
}

void ImageViewerProxy::openBase64(const QString& data) {
  const QByteArray img_data = QByteArray::fromBase64(data.toLocal8Bit());
  QPixmap pixmap;
  const bool status = pixmap.loadFromData(img_data);
  if (status) {
    emit this->openPixmapRequested(pixmap);
  } else {
    qWarning() << Q_FUNC_INFO << "Failed to load pixmap:" << data;
  }
}

}  // namespace dstore