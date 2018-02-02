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

#include <DTitlebar>
#include <QWebChannel>
#include <QWebEnginePage>
#include <QWebEngineSettings>

#include "base/consts.h"
#include "ui/image_viewer_proxy.h"
#include "ui/store_daemon_proxy.h"
#include "ui/widgets/image_viewer.h"
#include "ui/widgets/recommend_app.h"
#include "ui/widgets/search_completion_window.h"
#include "ui/widgets/title_bar.h"
#include "ui/widgets/tool_bar_menu.h"
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
  connect(tool_bar_menu_, &ToolBarMenu::recommendAppRequested,
          this, &WebWindow::onRecommendAppActive);
}

void WebWindow::initUI() {
  web_view_ = new WebView();
  this->setCentralWidget(web_view_);

  image_viewer_ = new ImageViewer(this);

  completion_window_ = new SearchCompletionWindow(this);
  completion_window_->hide();

  recommend_app_ = new RecommendApp(this);
  recommend_app_->hide();

  title_bar_ = new TitleBar();
  this->titlebar()->setCustomWidget(title_bar_, Qt::AlignCenter, false);
  this->titlebar()->setSeparatorVisible(true);
  tool_bar_menu_ = new ToolBarMenu(false, this);
  this->titlebar()->setMenu(tool_bar_menu_);

  // Disable web security.
  auto settings = web_view_->page()->settings();
  settings->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
  settings->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
  settings->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);
  settings->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
  settings->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard, true);

  auto web_channel = new QWebChannel(web_view_);
  web_view_->page()->setWebChannel(web_channel);
  image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
  store_daemon_proxy_ = new StoreDaemonProxy(this);
  web_channel->registerObject("imageViewer", image_viewer_proxy_);
  web_channel->registerObject("storeDaemon", store_daemon_proxy_);

  this->setFocusPolicy(Qt::ClickFocus);
}

void WebWindow::onRecommendAppActive() {
  recommend_app_->clearForm();
  recommend_app_->show();
}

}  // namespace dstore