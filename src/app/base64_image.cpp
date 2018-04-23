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

#include <QApplication>

#include <QDebug>
#include <QLabel>

#include "base/file_util.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  QByteArray data;
  dstore::ReadRawFile("/tmp/output.img", data);
  qDebug() << "data: " << data;
  const QByteArray img_data = QByteArray::fromBase64(data);
  qDebug() << "img data:" << img_data;
  QPixmap image;
  const bool status = image.loadFromData(img_data);
  qDebug() << "status: " << status;
  QLabel* label = new QLabel();
  label->setPixmap(image);
  label->show();

  return app.exec();
}