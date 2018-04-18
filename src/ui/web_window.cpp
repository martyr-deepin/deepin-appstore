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
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QWebChannel>
#include <qcef_web_page.h>
#include <qcef_web_settings.h>
#include <qcef_web_view.h>

#include "base/consts.h"
#include "services/search_manager.h"
#include "services/settings_manager.h"
#include "ui/web_event_delegate.h"
#include "ui/channel/image_viewer_proxy.h"
#include "ui/channel/log_proxy.h"
#include "ui/channel/search_proxy.h"
#include "ui/channel/settings_proxy.h"
#include "ui/channel/store_daemon_proxy.h"
#include "ui/widgets/image_viewer.h"
#include "ui/widgets/recommend_app.h"
#include "ui/widgets/search_completion_window.h"
#include "ui/widgets/title_bar.h"
#include "ui/widgets/title_bar_menu.h"

namespace dstore {

WebWindow::WebWindow(QWidget* parent) : DMainWindow(parent) {
  this->initUI();
  this->initServices();
  this->initProxy();

  // Connect signals to slots after all of internal objects are constructed.
  this->initConnections();
}

WebWindow::~WebWindow() {

}

void WebWindow::loadPage() {
  web_view_->load(QUrl(kIndexPage));
}

void WebWindow::showWindow() {
  this->setMinimumSize(872, 548);
  const QRect geometry = qApp->desktop()->availableGeometry(this);
  if (geometry.width() >= 1920) {
    this->resize(1208, 778);
  } else if (geometry.width() >= 1366) {
    this->resize(1108, 668);
  } else {
    this->resize(872, 548);
  }
  this->show();
}

void WebWindow::openApp(const QString& app_name) {
  qDebug() << Q_FUNC_INFO << app_name;
}

void WebWindow::raiseWindow() {
  this->raise();
}

void WebWindow::initConnections() {
  connect(search_manager_, &SearchManager::searchAppResult,
          this, &WebWindow::onSearchAppResult);

  connect(search_proxy_, &SearchProxy::onAppListUpdated,
          search_manager_, &SearchManager::updateAppList);

  connect(title_bar_, &TitleBar::backwardButtonClicked,
          this, &WebWindow::webViewGoBack);
  connect(title_bar_, &TitleBar::forwardButtonClicked,
          this, &WebWindow::webViewGoForward);
  connect(title_bar_, &TitleBar::searchTextChanged,
          search_manager_, &SearchManager::searchApp);

  connect(tool_bar_menu_, &TitleBarMenu::recommendAppRequested,
          this, &WebWindow::onRecommendAppActive);

  connect(web_view_->page(), &QCefWebPage::urlChanged,
          this, &WebWindow::onWebViewUrlChanged);
}

void WebWindow::initProxy() {
  auto web_channel = web_view_->page()->webChannel();
  image_viewer_proxy_ = new ImageViewerProxy(image_viewer_, this);
  log_proxy_ = new LogProxy(this);
  search_proxy_ = new SearchProxy(this);
  settings_proxy_ = new SettingsProxy(this);
  store_daemon_proxy_ = new StoreDaemonProxy(this);

  web_channel->registerObject("imageViewer", image_viewer_proxy_);
  web_channel->registerObject("log", log_proxy_);
  web_channel->registerObject("search", search_proxy_);
  web_channel->registerObject("settings", settings_proxy_);
  web_channel->registerObject("storeDaemon", store_daemon_proxy_);
}

void WebWindow::initUI() {
  web_view_ = new QCefWebView();
  this->setCentralWidget(web_view_);

  image_viewer_ = new ImageViewer(this);

  completion_window_ = new SearchCompletionWindow();
  completion_window_->hide();

  recommend_app_ = new RecommendApp(this);
  recommend_app_->hide();

  title_bar_ = new TitleBar();
  this->titlebar()->setCustomWidget(title_bar_, Qt::AlignCenter, false);
  this->titlebar()->setSeparatorVisible(true);
  tool_bar_menu_ = new TitleBarMenu(IsSignInSupported(), this);
  this->titlebar()->setMenu(tool_bar_menu_);

  // Disable web security.
  auto settings = web_view_->page()->settings();
  settings->setWebSecurity(QCefWebSettings::StateDisabled);

  web_event_delegate_ = new WebEventDelegate(this);
  web_view_->page()->setEventDelegate(web_event_delegate_);

  this->setFocusPolicy(Qt::ClickFocus);
}

void WebWindow::initServices() {
  search_manager_ = new SearchManager(this);
}

bool WebWindow::eventFilter(QObject* watched, QEvent* event) {
  // Filters mouse press event only.
  if (event->type() == QEvent::MouseButtonPress &&
      qApp->activeWindow() == this &&
      watched->objectName() == QLatin1String("QMainWindowClassWindow")) {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    switch (mouseEvent->button()) {
      case Qt::BackButton: {
        this->webViewGoBack();
        break;
      }
      case Qt::ForwardButton: {
        this->webViewGoForward();
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

void WebWindow::onRecommendAppActive() {
  recommend_app_->clearForm();
  recommend_app_->show();
}

void WebWindow::onSearchAppResult(const AppSearchRecordList& result) {
  if (result.isEmpty()) {
    // Hide completion window if no anchor entry matches.
    completion_window_->hide();
  } else {
    completion_window_->show();
    completion_window_->raise();
    completion_window_->autoResize();
    // Move to below of search edit.
    const QPoint local_point(this->rect().width() / 2 - 94, 36);
    const QPoint global_point(this->mapToGlobal(local_point));
    completion_window_->move(global_point);
    completion_window_->setFocusPolicy(Qt::NoFocus);
    completion_window_->setFocusPolicy(Qt::StrongFocus);
    completion_window_->setSearchAnchorResult(result);
  }
}

void WebWindow::onWebViewUrlChanged(const QUrl& url) {
  Q_UNUSED(url);
  auto page = web_view_->page();
  title_bar_->setBackwardButtonActive(page->canGoBack());
  title_bar_->setForwardButtonActive(page->canGoForward());
}

void WebWindow::webViewGoBack() {
  auto page = web_view_->page();
  if (page->canGoBack()) {
    page->back();
  }
}

void WebWindow::webViewGoForward() {
  auto page = web_view_->page();
  if (page->canGoForward()) {
    page->forward();
  }
}

}  // namespace dstore