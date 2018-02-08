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

#include <DApplication>
#include <DLog>
#include <QIcon>
#include <QWebEngineProfile>

#include "base/consts.h"
#include "resources/images.h"
#include "ui/web_window.h"

int main(int argc, char** argv) {
  Dtk::Widget::DApplication::loadDXcbPlugin();

  Dtk::Widget::DApplication app(argc, argv);

  app.setTheme("light");
  app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
  app.setWindowIcon(QIcon(dstore::kImageDeepinAppStore));
  app.setProductIcon(QIcon(dstore::kImageDeepinAppStore));
  app.setOrganizationName("deepin");
  app.setOrganizationDomain("deepin.org");
  app.setApplicationVersion(dstore::kAppVersion);
  app.setApplicationName(dstore::kAppName);
  app.loadTranslator();
  app.setApplicationDisplayName(QObject::tr("Deepin App Store"));
  app.setApplicationDescription(QObject::tr(
      "Deepin Store is an Appstore with quality and rich applications. "
          "Hot recommendation, new arrivals and topic introduction "
          "are available. It supports one click to download, install, "
          "update, uninstall and so on."));
  app.setApplicationAcknowledgementPage(
      "https://www.deepin.org/acknowledgments/deepin-appstore/");

  Dtk::Core::DLogManager::registerConsoleAppender();
  Dtk::Core::DLogManager::registerFileAppender();

  QWebEngineProfile* profile = QWebEngineProfile::defaultProfile();
  QDir cache_dir = dstore::GetCacheDir();
  profile->setCachePath(cache_dir.filePath("cache"));
  profile->setPersistentStoragePath(cache_dir.filePath("storage"));

  dstore::WebWindow window;
  window.loadPage();
  window.show();

  return app.exec();
}