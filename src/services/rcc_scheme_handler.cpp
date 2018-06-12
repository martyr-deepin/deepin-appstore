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

#include "services/rcc_scheme_handler.h"

#include <QDebug>
#include <QFileInfo>
#include <QLocale>

#include "services/metadata_manager.h"

namespace dstore {

namespace {

MetadataManager* g_metadata_manager = nullptr;

}  // namespace

QString RccSchemeHandler(const QUrl& url) {
  if (g_metadata_manager == nullptr) {
    g_metadata_manager = new MetadataManager();
    g_metadata_manager->downloadAppIcons();
  }

  const QString host = url.host();
  if (host == "web") {
    const char kAppDefaultLocalDir[] = DSTORE_WEB_DIR "/appstore";
    QString app_local_dir = QString("%1/appstore-%2")
        .arg(DSTORE_WEB_DIR)
        .arg(QLocale().name());
    if (!QFileInfo::exists(app_local_dir)) {
      app_local_dir = kAppDefaultLocalDir;
    }

    QString filepath = QString("%1/%2").arg(app_local_dir).arg(url.path());
    if (!QFileInfo::exists(filepath)) {
      filepath = QString("%1/%2").arg(app_local_dir).arg("index.html");
    }
    return filepath;
  } else if (host == "icon") {
    const QString app_name = url.fileName();
    const QString icon_path = g_metadata_manager->getAppIcon(app_name);
    return icon_path;
  } else {
    // 404 not found.
    return "";
  }
}

}  // namespace dstore
