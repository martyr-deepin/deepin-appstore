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

#ifndef DEEPIN_APPSTORE_UI_IMAGE_VIEWER_PROXY_H
#define DEEPIN_APPSTORE_UI_IMAGE_VIEWER_PROXY_H

#include <QObject>

namespace dstore {

class ImageViewerProxy : public QObject {
  Q_OBJECT
 public:
  explicit ImageViewerProxy(QObject* parent = nullptr);
  ~ImageViewerProxy() override;

 signals:
  void openImageFileRequested(const QString& filepath);

  void openPixmapRequested(const QPixmap& pixmap);

 public slots:
  /**
   * Open image in viewer window.
   * @param filepath Absolute path to image file.
   */
  void open(const QString& filepath);

  /**
   * Open base64-encoded image data.
   * @param data
   */
  void openBase64(const QString& data);
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_IMAGE_VIEWER_PROXY_H
