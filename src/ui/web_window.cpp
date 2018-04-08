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

#include "ui/web_window.h"

#include <DTitlebar>
#include <QApplication>
#include <QDebug>
#include <QResizeEvent>
#include <QWebChannel>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

#include "base/consts.h"
#include "services/settings_manager.h"
#include "ui/channel/image_viewer_proxy.h"
#include "ui/channel/store_daemon_proxy.h"
#include "ui/channel/title_bar_proxy.h"
#include "ui/widgets/image_viewer.h"
#include "ui/widgets/recommend_app.h"
#include "ui/widgets/search_completion_window.h"
#include "ui/widgets/title_bar.h"
#include "ui/widgets/tool_bar_menu.h"

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

void WebWindow::openApp(const QString& app_name) {
  Q_UNUSED(app_name);
}

void WebWindow::initConnections() {
  connect(tool_bar_menu_, &ToolBarMenu::recommendAppRequested,
          this, &WebWindow::onRecommendAppActive);
}

void WebWindow::initUI() {
  web_view_ = new QCefWebView();
  this->setCentralWidget(web_view_);

  image_viewer_ = new ImageViewer(this);

  completion_window_ = new SearchCompletionWindow(this);
  completion_window_->hide();

  recommend_app_ = new RecommendApp(this);
  recommend_app_->hide();

  title_bar_ = new TitleBar();
  this->titlebar()->setCustomWidget(title_bar_, Qt::AlignCenter, false);
  this->titlebar()->setSeparatorVisible(true);
  tool_bar_menu_ = new ToolBarMenu(IsSignInSupported(), this);
  this->titlebar()->setMenu(tool_bar_menu_);

  // Disable web security.
  auto settings = web_view_->page()->settings();
  settings->setWebSecurity(QCefWebSettings::StateDisabled);

  auto web_channel = web_view_->page()->webChannel();
  title_bar_proxy_ = new TitleBarProxy(this);
  image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
  store_daemon_proxy_ = new StoreDaemonProxy(this);
  web_channel->registerObject("imageViewer", image_viewer_proxy_);
  web_channel->registerObject("storeDaemon", store_daemon_proxy_);
  web_channel->registerObject("titleBar", title_bar_proxy_);

  this->setFocusPolicy(Qt::ClickFocus);

  this->resize(800, 600);
}

void WebWindow::onRecommendAppActive() {
  recommend_app_->clearForm();
  recommend_app_->show();
}

bool WebWindow::eventFilter(QObject* watched, QEvent* event) {
  // Filters mouse press event only.
  if (event->type() == QEvent::MouseButtonPress &&
      qApp->activeWindow() == this &&
      watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    switch (mouseEvent->button()) {
      case Qt::BackButton: {
        title_bar_proxy_->backwardButtonClicked();
        break;
      }
      case Qt::ForwardButton: {
        title_bar_proxy_->forwardButtonClicked();
        break;
      }
      default: {
      }
    }
  }
  return QObject::eventFilter(watched, event);
}

void WebWindow::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  title_bar_->setFixedWidth(event->size().width());
}

}  // namespace dstore