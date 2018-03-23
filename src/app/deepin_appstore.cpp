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
#include <qcef_context.h>
#include <qcef_web_settings.h>
#include <DPlatformWindowHandle>

#include "base/consts.h"
#include "resources/images.h"
#include "services/args_parser.h"
#include "ui/web_window.h"

int main(int argc, char** argv) {
  qputenv("DXCB_FAKE_PLATFORM_NAME_XCB", "true");
  qputenv("DXCB_REDIRECT_CONTENT", "true");

  QCefGlobalSettings settings;
  // Do not use sandbox.
  settings.setNoSandbox(true);
#ifndef NDEBUG
  // Open http://localhost:9222 in chromium browser to see dev tools.
  settings.setRemoteDebug(true);
  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Warning);
#else
  settings.setRemoteDebug(false);
  settings.setLogSeverity(QCefGlobalSettings::LogSeverity::Error);
#endif
  // Disable GPU process.
  settings.addCommandLineSwitch("--disable-gpu", "");
  // Set web cache folder.
  QDir cache_dir(dstore::GetCacheDir());
  cache_dir.mkpath(".");
  settings.setCachePath(cache_dir.filePath("cache"));
  settings.setUserDataPath(cache_dir.filePath("cef-storage"));

  if (QCefInit(argc, argv, settings) >= 0) {
    return 0;
  }

  Dtk::Widget::DApplication::loadDXcbPlugin();
  Dtk::Widget::DApplication app(argc, argv);
  if (!Dtk::Widget::DPlatformWindowHandle::pluginVersion().isEmpty()) {
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings, true);
  }

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

  dstore::ArgsParser parser;
  if (parser.parseArguments()) {
    // Exit process after 1000ms.
    QTimer::singleShot(1000, [&]() {
      app.quit();
    });
    return app.exec();
  } else {
    QCefBindApp(&app);

    dstore::WebWindow window;
    QObject::connect(&parser, &dstore::ArgsParser::openAppRequested,
                     &window, &dstore::WebWindow::openApp);

    window.loadPage();
    window.show();
    parser.openAppDelay();

    return app.exec();
  }
}