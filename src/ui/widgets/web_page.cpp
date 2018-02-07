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

#include "ui/widgets/web_page.h"

#include <QDesktopServices>

namespace dstore {

WebPage::WebPage(QObject* parent)
    : QWebEnginePage(parent),
      window_type_(QWebEnginePage::WebWindowType::WebBrowserWindow) {
}

WebPage::~WebPage() {

}

QWebEnginePage* WebPage::createWindow(QWebEnginePage::WebWindowType type) {
  window_type_ = type;
  return this;
}

bool WebPage::acceptNavigationRequest(const QUrl& url,
                                      QWebEnginePage::NavigationType type,
                                      bool isMainFrame) {
  switch (window_type_) {
    case WebBrowserBackgroundTab: // fall through
    case WebBrowserTab: {
      QDesktopServices::openUrl(url);
      return false;
    }
    default: {
      return QWebEnginePage::acceptNavigationRequest(url, type, isMainFrame);
    }
  }
}

}  // namespace dstore