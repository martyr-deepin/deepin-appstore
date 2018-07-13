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

void ImageViewerProxy::setImageList(const QStringList& urls, int current) {
  urls_ = urls;
  current_ = -1;
  if (current >= 0 && current < urls.length()) {
    current_ = current;
    // Notify web page to download current image.
    emit this->openOnlineImageRequest(urls.at(current));
  } else {
    qWarning() << Q_FUNC_INFO << "Invalid url index:" << current << urls;
  }
}

void ImageViewerProxy::openBase64(const QString& url, const QString& data) {
  Q_ASSERT(current_ < urls_.length());
  if (url != urls_.at(current_)) {
    qWarning() << Q_FUNC_INFO << "url not match,"
               <<"expected:" << urls_.at(current_)
               << ", got:" << url;
    return;
  }

  const QByteArray img_data = QByteArray::fromBase64(data.toLocal8Bit());
  QPixmap pixmap;
  const bool status = pixmap.loadFromData(img_data);
  if (status) {
    emit this->openPixmapRequested(pixmap);
  } else {
    qWarning() << Q_FUNC_INFO << "Failed to load pixmap:" << url;
  }
}

void ImageViewerProxy::onPreviousImageRequested() {
  current_ = (urls_.length() - current_ + 1) % urls_.length();
  qDebug() << Q_FUNC_INFO << current_ << urls_.length();
  emit this->openOnlineImageRequest(urls_.at(current_));
}

void ImageViewerProxy::onNextImageRequested() {
  current_ = (current_ + 1) % urls_.length();
  qDebug() << Q_FUNC_INFO << current_ << urls_.length();
  emit this->openOnlineImageRequest(urls_.at(current_));
}

}  // namespace dstore