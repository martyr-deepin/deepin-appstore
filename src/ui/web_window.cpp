/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include "ui/web_window.h"

#include <QWebChannel>
#include <QWebEnginePage>

#include "base/consts.h"
#include "ui/image_viewer_proxy.h"
#include "ui/store_daemon_proxy.h"
#include "ui/widgets/image_viewer.h"
#include "ui/widgets/web_view.h"

namespace dstore {

WebWindow::WebWindow(QWidget* parent) : DMainWindow(parent) {
  this->initUI();
  this->initConnections();
}

WebWindow::~WebWindow() {

}

void WebWindow::loadPage() {
  web_view_->load(QUrl::fromLocalFile(kIndexPage));
}

void WebWindow::initConnections() {

}

void WebWindow::initUI() {
  web_view_ = new WebView();
  this->setCentralWidget(web_view_);

  image_viewer_ = new ImageViewer(this);

  // Disable web security.

  QWebChannel* web_channel = new QWebChannel(web_view_);
  web_view_->page()->setWebChannel(web_channel);
  image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
  store_daemon_proxy_ = new StoreDaemonProxy(this);
  web_channel->registerObject("imageViewer", image_viewer_proxy_);
  web_channel->registerObject("storeDaemon", store_daemon_proxy_);

  this->setFocusPolicy(Qt::ClickFocus);
}

}  // namespace dstore