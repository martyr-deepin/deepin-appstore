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

#include "ui/web_event_delegate.h"

#include <qcef_web_page.h>

namespace dstore {

namespace {

enum MenuIds {
  // Normal navigation.
  MenuBack = QCefContextMenu::MENU_ID_USER_FIRST,
  MenuForward,
  MenuReload,
  MenuStop,

  // Editable.
  MenuUndo,
  MenuRedo,
  MenuCut,
  MenuCopy,
  MenuPaste,
  MenuDelete,
  MenuSelectAll,

  // Link.
  MenuOpenLinkInNewTab,
  MenuCopyLinkAddress,
};

}  // namespace

WebEventDelegate::WebEventDelegate(QObject* parent) : QObject(parent) {
  this->setObjectName("WebEventDelegate");
}

WebEventDelegate::~WebEventDelegate() {

}

bool WebEventDelegate::onBeforeBrowse(const QUrl& url, bool is_redirect) {
  return QCefBrowserEventDelegate::onBeforeBrowse(url, is_redirect);
}

void WebEventDelegate::onBeforeContextMenu(
    QCefWebPage* web_page,
    QCefContextMenu* menu,
    const QCefContextMenuParams& params) {
  QCefBrowserEventDelegate::onBeforeContextMenu(web_page, menu, params);
  auto type_flags = params.getTypeFlags();
  if (type_flags & QCEF_CM_FLAG_EDITABLE) {

  }

  if (params.isEditable()) {
    // Editable menu.
    auto state = params.getEditStateFlags();
    menu->addItem(MenuIds::MenuUndo, QObject::tr("Undo"),
                  state & QCEF_CM_EDITFLAG_CAN_UNDO,
                  [](QCefWebPage* page) {
                    page->undo();
                  });
    menu->addItem(MenuIds::MenuRedo, QObject::tr("Redo"),
                  state & QCEF_CM_EDITFLAG_CAN_REDO,
                  [](QCefWebPage* page) {
                    page->redo();
                  });
    menu->addSeparator();
    menu->addItem(MenuIds::MenuCut, QObject::tr("Cut"),
                  state & QCEF_CM_EDITFLAG_CAN_CUT,
                  [](QCefWebPage* page) {
                    page->cut();
                  });
    menu->addItem(MenuIds::MenuCopy, QObject::tr("Copy"),
                  state & QCEF_CM_EDITFLAG_CAN_COPY,
                  [](QCefWebPage* page) {
                    page->copy();
                  });
    menu->addItem(MenuIds::MenuPaste, QObject::tr("Paste"),
                  state & QCEF_CM_EDITFLAG_CAN_PASTE,
                  [](QCefWebPage* page) {
                    page->paste();
                  });
    menu->addItem(MenuIds::MenuDelete, QObject::tr("Delete"),
                  state & QCEF_CM_EDITFLAG_CAN_DELETE,
                  [](QCefWebPage* page) {
                    page->doDelete();
                  });
    menu->addSeparator();
    menu->addItem(MenuIds::MenuSelectAll, QObject::tr("Select all"),
                  state & QCEF_CM_EDITFLAG_CAN_SELECT_ALL,
                  [](QCefWebPage* page) {
                    page->selectAll();
                  });
    return;
  }

}

bool WebEventDelegate::onBeforePopup(const QUrl& url,
                                     QCefWindowOpenDisposition disposition) {
  return QCefBrowserEventDelegate::onBeforePopup(url, disposition);
}

}  // namespace dstore