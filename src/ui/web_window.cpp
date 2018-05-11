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
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QTimer>
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
#include "ui/channel/menu_proxy.h"
#include "ui/channel/search_proxy.h"
#include "ui/channel/settings_proxy.h"
#include "ui/channel/store_daemon_proxy.h"
#include "ui/widgets/image_viewer.h"
#include "ui/widgets/search_completion_window.h"
#include "ui/widgets/title_bar.h"
#include "ui/widgets/title_bar_menu.h"

namespace dstore {

namespace {

const int kSearchDelay = 200;

}  // namespace

WebWindow::WebWindow(QWidget* parent)
    : DMainWindow(parent),
      search_timer_(new QTimer(this)) {
  this->setObjectName("WebWindow");

  search_timer_->setSingleShot(true);

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

void WebWindow::showAppDetail(const QString& app_name) {
  qDebug() << Q_FUNC_INFO << app_name;
  // TODO(Shaohua): Make sure angular context has been initialized.
  emit search_proxy_->openApp(app_name);
}

void WebWindow::raiseWindow() {
  this->raise();
}

void WebWindow::initConnections() {
  connect(completion_window_, &SearchCompletionWindow::resultClicked,
          this, &WebWindow::onSearchResultClicked);
  connect(completion_window_, &SearchCompletionWindow::searchButtonClicked,
          this, &WebWindow::onSearchButtonClicked);

  connect(image_viewer_proxy_, &ImageViewerProxy::openImageFileRequested,
          image_viewer_, &ImageViewer::open);
  connect(image_viewer_proxy_, &ImageViewerProxy::openPixmapRequested,
          image_viewer_, &ImageViewer::openPixmap);

  connect(search_manager_, &SearchManager::searchAppResult,
          this, &WebWindow::onSearchAppResult);

  connect(search_proxy_, &SearchProxy::onAppListUpdated,
          search_manager_, &SearchManager::updateAppList);
  connect(search_proxy_, &SearchProxy::onAppListUpdated,
          store_daemon_proxy_, &StoreDaemonProxy::updateAppList);

  connect(search_timer_, &QTimer::timeout,
          this, &WebWindow::onSearchTextChangedDelay);

  connect(title_bar_, &TitleBar::backwardButtonClicked,
          this, &WebWindow::webViewGoBack);
  connect(title_bar_, &TitleBar::forwardButtonClicked,
          this, &WebWindow::webViewGoForward);
  connect(title_bar_, &TitleBar::searchTextChanged,
          this, &WebWindow::onSearchTextChanged);
  connect(title_bar_, &TitleBar::downKeyPressed,
          completion_window_, &SearchCompletionWindow::goDown);
  connect(title_bar_, &TitleBar::enterPressed,
          this, &WebWindow::onTitleBarEntered);
  connect(title_bar_, &TitleBar::upKeyPressed,
          completion_window_, &SearchCompletionWindow::goUp);
  connect(title_bar_, &TitleBar::focusOut,
          this, &WebWindow::onSearchEditFocusOut);

  connect(tool_bar_menu_, &TitleBarMenu::recommendAppRequested,
          menu_proxy_, &MenuProxy::recommendAppRequested);
  connect(tool_bar_menu_, &TitleBarMenu::loginRequested,
          menu_proxy_, &MenuProxy::loginRequested);
  connect(menu_proxy_, &MenuProxy::loginStateUpdated,
          tool_bar_menu_, &TitleBarMenu::setLoginState);

  connect(web_view_->page(), &QCefWebPage::urlChanged,
          this, &WebWindow::onWebViewUrlChanged);
}

void WebWindow::initProxy() {
  auto web_channel = web_view_->page()->webChannel();
  image_viewer_proxy_ = new ImageViewerProxy(this);
  log_proxy_ = new LogProxy(this);
  menu_proxy_ = new MenuProxy(this);
  search_proxy_ = new SearchProxy(this);
  settings_proxy_ = new SettingsProxy(this);
  store_daemon_proxy_ = new StoreDaemonProxy(this);

  web_channel->registerObject("imageViewer", image_viewer_proxy_);
  web_channel->registerObject("log", log_proxy_);
  web_channel->registerObject("menu", menu_proxy_);
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

void WebWindow::onSearchAppResult(const QString& keyword,
                                  bool entered,
                                  const AppSearchRecordList& result) {
  Q_UNUSED(keyword);
  completion_window_->setSearchResult(result);

  if (entered) {
    // Show search page in web.
    QStringList names;
    for (const AppSearchRecord& app : completion_window_->searchResult()) {
      names.append(app.name);
    }
    emit search_proxy_->openAppList(completion_window_->getKeyword(), names);
    completion_window_->hide();
  } else {
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
    }
  }
}

void WebWindow::onSearchEditFocusOut() {
  QTimer::singleShot(20, [=]() {
    this->completion_window_->hide();
  });
}

void WebWindow::onSearchButtonClicked() {
  this->prepareSearch(true);
}

void WebWindow::onSearchTextChangedDelay() {
  this->prepareSearch(false);
}

void WebWindow::prepareSearch(bool entered) {
  const QString text = title_bar_->getSearchText();
  // Filters special chars.
  if (text.size() <= 1 ||
      text.contains(QRegularExpression("[+_-$!@#%^&\\(\\)]"))) {
    return;
  }

  completion_window_->setKeyword(text);

  // Do real search.
  search_manager_->searchApp(text, entered);
}

void WebWindow::onSearchResultClicked(const AppSearchRecord& result) {
  // Emit signal to web page.
  emit search_proxy_->openApp(result.name);
}

void WebWindow::onSearchTextChanged(const QString& text) {
  if (text.size() > 1) {
    search_timer_->stop();
    search_timer_->start(kSearchDelay);
  } else {
    this->onSearchEditFocusOut();
  }
}

void WebWindow::onTitleBarEntered() {
  const QString text = title_bar_->getSearchText();
  if (text.size() > 1) {
    completion_window_->onEnterPressed();
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
