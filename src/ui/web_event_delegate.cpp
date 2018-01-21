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

#include "ui/web_event_delegate.h"

#include <QDesktopServices>

namespace dstore {

WebEventDelegate::WebEventDelegate(QObject* parent) : QObject(parent) {

}

WebEventDelegate::~WebEventDelegate() {

}

bool WebEventDelegate::onBeforeBrowse(const QUrl& url, bool is_redirect) {
  return QCefBrowserEventDelegate::onBeforeBrowse(url, is_redirect);
}

void WebEventDelegate::onBeforeContextMenu(QCefWebPage* web_page,
                                           QCefContextMenu* menu,
                                           const QCefContextMenuParams& params) {
  QCefBrowserEventDelegate::onBeforeContextMenu(web_page, menu, params);
}

bool WebEventDelegate::onBeforePopup(const QUrl& url,
                                     QCefWindowOpenDisposition disposition) {
  if (disposition == QCefWindowOpenDisposition::QCEF_WOD_NEW_BACKGROUND_TAB ||
      disposition == QCefWindowOpenDisposition::QCEF_WOD_NEW_FOREGROUND_TAB) {
    QDesktopServices::openUrl(url);
  }
  return QCefBrowserEventDelegate::onBeforePopup(url, disposition);
}

}  // namespace dstore