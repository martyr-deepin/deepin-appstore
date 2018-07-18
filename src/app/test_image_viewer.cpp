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
#include <QTimer>

#include "ui/widgets/image_viewer.h"

int main(int argc, char** argv) {
  QApplication app(argc, argv);

  dstore::ImageViewer* viewer = new dstore::ImageViewer();
  QObject::connect(viewer, &dstore::ImageViewer::previousImageRequested, [&]() {
    viewer->showIndicator();
    QTimer::singleShot(2000, [=]() {
      viewer->open("/tmp/demo2.jpg");
    });
  });

  QObject::connect(viewer, &dstore::ImageViewer::nextImageRequested, [&]() {
    viewer->showIndicator();
    QTimer::singleShot(2000, [=]() {
      viewer->open("/tmp/demo.jpg");
    });
  });

  viewer->show();

  return app.exec();
}