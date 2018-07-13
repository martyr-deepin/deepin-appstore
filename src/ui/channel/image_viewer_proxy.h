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

// Image viewer controller
class ImageViewerProxy : public QObject {
  Q_OBJECT
 public:
  explicit ImageViewerProxy(QObject* parent = nullptr);
  ~ImageViewerProxy() override;

 signals:
  void openImageFileRequested(const QString& filepath);

  void openPixmapRequested(const QPixmap& pixmap);

  /**
   * Request to open online image file.
   * Call |openBase64| to send response.
   * @param url Image url
   */
  void openOnlineImageRequest(const QString& url);

 public slots:
  /**
   * Open image in viewer window.
   * @param filepath Absolute path to image file.
   */
  void open(const QString& filepath);

  /**
   * Update image url list.
   * Call this method
   * @param urls
   * @param current Index of image to be open.
   */
  void setImageList(const QStringList& urls, int current);

  /**
   * Open base64-encoded image data.
   * @param url Image url
   * @param data Base64-encoded image content
   */
  void openBase64(const QString& url, const QString& data);

  void onPreviousImageRequested();
  void onNextImageRequested();

 private:
  QStringList urls_;
  int current_;
};

}  // namespace dstore

#endif  // DEEPIN_APPSTORE_UI_IMAGE_VIEWER_PROXY_H
