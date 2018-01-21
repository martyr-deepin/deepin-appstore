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

#include <qcef_web_page.h>
#include <qcef_web_settings.h>

#include "base/consts.h"

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
  web_view_ = new QCefWebView();
  this->setCentralWidget(web_view_);
  web_event_delegate_ = new WebEventDelegate(this);
  web_view_->page()->setEventDelegate(web_event_delegate_);
  // Disable web security.
  web_view_->page()->settings()->setWebSecurity(QCefWebSettings::StateDisabled);

  this->setFocusPolicy(Qt::ClickFocus);
}

}  // namespace dstore