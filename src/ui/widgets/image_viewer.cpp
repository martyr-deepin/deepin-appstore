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

#include "ui/widgets/image_viewer.h"

#include <DWidgetUtil>
#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QResizeEvent>
#include <QStackedLayout>
#include <QtCore/QTimer>
#include <QDesktopWidget>

#include "ui/utils/theme_manager.h"

namespace dstore {

namespace {

const int kBorderSize = 12;
const int kCloseBtnSize = 48;

}  // namespace

ImageViewer::ImageViewer(QWidget* parent)
    : QDialog(parent) {
  this->setObjectName("ImageViewer");
  this->initUI();

  connect(close_button_, &Dtk::Widget::DImageButton::clicked,
          this, &ImageViewer::close);
}

ImageViewer::~ImageViewer() {

}

void ImageViewer::open(const QString& filepath) {
  // Escape uri.
  QString abspath(filepath);
  if (abspath.contains("://")) {
    QUrl url(abspath);
    abspath = url.path();
  }

  QPixmap pixmap(abspath);
  const QRect screen_rect = qApp->desktop()->screenGeometry(QCursor::pos());
  // Resize image to fix screen.
  const int pixmap_max_width = static_cast<int>(screen_rect.width() * 0.8);
  const int pixmap_max_height = static_cast<int>(screen_rect.height() * 0.8);
  if ((pixmap.width() > pixmap_max_width) ||
      (pixmap.height() > pixmap_max_height)) {
    pixmap = pixmap.scaled(pixmap_max_width,
                           pixmap_max_height,
                           Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);
  }

  img_label_->setPixmap(pixmap);
  img_label_->setFixedSize(pixmap.width(), pixmap.height());
  QRect img_rect = img_label_->rect();
  img_rect.moveCenter(screen_rect.center());
  img_label_->move(img_rect.topLeft());

  // Move close button to top-right corner of image.
  const QPoint top_right_point = img_rect.topRight();
  close_button_->move(top_right_point.x() - kCloseBtnSize / 2,
                      top_right_point.y() - kCloseBtnSize / 2);
  close_button_->show();
  close_button_->raise();
  this->showFullScreen();
}

void ImageViewer::initUI() {
  img_label_ = new QLabel(this);
  img_label_->setObjectName("ImageLabel");

  close_button_ = new Dtk::Widget::DImageButton(this);
  close_button_->setObjectName("CloseButton");
  close_button_->raise();

  this->setContentsMargins(kBorderSize, kBorderSize, kBorderSize, kBorderSize);
  this->setWindowFlags(Qt::FramelessWindowHint |
                       Qt::BypassWindowManagerHint |
                       Qt::Dialog |
                       Qt::WindowStaysOnTopHint);
  this->setAttribute(Qt::WA_TranslucentBackground, true);
  this->setModal(true);

  ThemeManager::instance()->registerWidget(this);
}

void ImageViewer::mousePressEvent(QMouseEvent* event) {
  QWidget::mousePressEvent(event);
  this->hide();
}

void ImageViewer::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  painter.fillRect(this->geometry(), QColor(0, 0, 0, 77));
}

}  // namespace dstore