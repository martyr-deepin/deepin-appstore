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

#include <DApplication>
#include <QIcon>
#include <qcef_context.h>

#include "base/consts.h"
#include "resources/images.h"

int main(int argc, char** argv) {
  QCefGlobalSettings settings;

  // Do not use sandbox.
  settings.setNoSandbox(true);
//#ifndef N_DEBUG
  // Open http://localhost:9222 in chromium browser to see dev tools.
  settings.setRemoteDebug(true);
  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Error);
//#else
//  settings.setRemoteDebug(false);
//  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Error);
//#endif

  // Disable GPU process.
  settings.addCommandLineSwitch("--disable-gpu", "");
  // Set web cache folder.
  QDir cache_dir(dstore::GetCacheDir());
  cache_dir.mkpath(".");
  settings.setCachePath(cache_dir.filePath("cache"));
  settings.setUserDataPath(cache_dir.filePath("data"));

  const int exit_code = QCefInit(argc, argv, settings);
  if (exit_code >= 0) {
    return exit_code;
  }

  Dtk::Widget::DApplication::loadDXcbPlugin();

  Dtk::Widget::DApplication app(argc, argv);
  QCefBindApp(&app);

  app.setTheme("light");
  app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
  app.setWindowIcon(QIcon(dstore::kImageDeepinAppStore));
  app.setProductIcon(QIcon(dstore::kImageDeepinAppStore));
  app.setOrganizationName("deepin");
  app.setOrganizationDomain("deepin.org");
  app.setApplicationVersion(dstore::kAppVersion);
  app.setApplicationName(dstore::kAppName);
  app.loadTranslator();
  app.setApplicationDisplayName(QObject::tr(dstore::kAppDisplayName));
  app.setApplicationDescription(QObject::tr("Deepin app store"));
  app.setApplicationAcknowledgementPage(
      "https://www.deepin.org/acknowledgments/deepin-appstore/");

  return app.exec();
}