/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#define QT_NO_KEYWORDS
    // for opening .desktop files
    #include <gio/gio.h>
    #include <gio/gdesktopappinfo.h>

    // for locales
    #include <glib.h>
    #include <glib/gi18n.h>
#undef QT_NO_KEYWORDS

#include "common.h"
#include <cassert>
#include <QApplication>

// for tooltips
#include <QFont>
#include <QFontMetrics>
#include <QLabel>

#include <QLayout>

#include "Shell.h"
#include "Bridge.h"
#include "WebWidget.h"

#include <QDesktopServices>
#include "MainWindow.h"
#include "AboutWindow.h"
#include "dbusmenu.h"
#include "dbusmenumanager.h"

auto nameWindowState(Qt::WindowStates state) -> QString {
    if (state & Qt::WindowMaximized) {
        return "maximized";
    } else if (state & Qt::WindowMinimized) {
        return "minimized";
    } else {
        return "normal";
    }
}

Bridge::Bridge(QObject *parent) : QObject(parent) {
    this->lastore = new LAStoreBridge(this);
    this->menuManager = new DBusMenuManager(this);

    // bind window state change
    const auto mainWin = this->getMainWindow();
    connect(mainWin, &MainWindow::windowStateChanged,
            this, [this](Qt::WindowState state) {
                emit this->windowStateAnswered(nameWindowState(state));
            });

    this->calcLanguages(); // may or may not contain blocking code
    this->calcTimezone(); // contains blocking code
    this->calcAppRegion(); // no blocking code
}

Bridge::~Bridge() {
    this->unregisterMenu();
    if (this->lastore) {
        delete this->lastore;
        this->lastore = nullptr;
    }
    if (this->menuManager) {
        delete this->menuManager;
        this->menuManager = nullptr;
    }
}

void Bridge::exit() {
    qApp->exit();
}

void Bridge::showMinimize() {
    this->getMainWindow()->showMinimized();
}

void Bridge::toggleMaximized() {
    this->getMainWindow()->toggleMaximized();
}

void Bridge::showTooltip(const QString& text,
                         const int& x, const int& y,
                         const int& w, const int& h) {
    const auto mainWindow = this->getMainWindow();
    const auto globalPos = mainWindow->mapToGlobal(QPoint(x, y));
    static_cast<Shell*>(qApp)->showTooltip(text,
                                           QRect(globalPos.x(), globalPos.y(), w, h));
}


void Bridge::startMoving() {
    this->getMainWindow()->startMoving();
}


MainWindow* Bridge::getMainWindow() {
    WebPage* webPage = static_cast<WebPage*>(this->parent());
    WebView* webView = static_cast<WebView*>(webPage->parent());
    MainWindow* mainWindow = static_cast<MainWindow*>(webView->window());
    return mainWindow;
}

// Window Menu
void Bridge::unregisterMenu() {
    if (this->menuPath.size()) {
        this->menuManager->UnregisterMenu(this->menuPath);
    }
}

void Bridge::onMenuUnregistered() {
    if (this->menu) {
        delete this->menu;
        this->menu = nullptr;
    }
    if (this->menuPath.size()) {
        this->menuPath = "";
    }
}

void Bridge::showMenu(QVariantMap content) {
    if (this->menuPath.size()) {
        qWarning() << "Another menu is active";
        return;
    }

    // map coordinates
    const auto coord = QPoint((int)content["x"].toDouble(), (int)content["y"].toDouble());
    const auto translatedCoord = this->getMainWindow()->mapToGlobal(coord);
    content["x"] = translatedCoord.x();
    content["y"] = translatedCoord.y();

    // stringify JSON
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(content);
    const auto menuStr = jsonDoc.toJson();

    asyncWatcherFactory<QDBusObjectPath>(
        this->menuManager->RegisterMenu(),
        [this, menuStr](QDBusPendingReply<QDBusObjectPath> reply) {
            this->menuPath = reply.argumentAt<0>().path();
            this->menu = new DBusMenu(this->menuPath, this);
            connect(this->menu, &DBusMenu::MenuUnregistered,
                    this, &Bridge::onMenuUnregistered);

            connect(this->menu, &DBusMenu::ItemInvoked,
                    this, &Bridge::onItemInvoked);
            this->menu->ShowMenu(menuStr);
        }
    );
}

void Bridge::onItemInvoked(const QString& id, bool UNUSED(checked)) {
    if (id == "exit") {
        qApp->exit(0);
    } else if (id == "help") {
        const auto shell = static_cast<Shell*>(qApp);
        shell->openManual();
    } else if (id == "about") {
        this->showAboutWindow();
    } else if (id == "logout") {
        emit this->logoutRequested();
    } else if (id == "login") {
        emit this->loginRequested();
    }
}

void Bridge::openExternalBrowser(const QString& url) {
    QDesktopServices::openUrl(QUrl(url));
}

void Bridge::openDesktopFile(const QString& path) {
    if (path == "") {
         return;
    }
    auto connection = QDBusConnection::sessionBus();
    auto msg = QDBusMessage::createMethodCall("com.deepin.SessionManager",
                                              "/com/deepin/StartManager",
                                              "com.deepin.StartManager",
                                              "Launch");
    msg << path;

    const auto fallbackOpenDesktopFile = [path]() {
        // fallback to gio
        if (path.isEmpty()) {
            qDebug() << "Failed to open desktop file with gio: desktop file path is empty";
            return;
        }

        const auto stdPath = path.toStdString();
        const char* cPath = stdPath.c_str();

        GDesktopAppInfo* appInfo = g_desktop_app_info_new_from_filename(cPath);
        if (!appInfo) {
            qDebug() << "Failed to open desktop file with gio: g_desktop_app_info_new_from_filename returns NULL. Check PATH maybe?";
            return;
        }
        GError* gError = nullptr;
        const auto ok = g_app_info_launch_uris(reinterpret_cast<GAppInfo*>(appInfo), NULL, NULL, &gError);

        if (gError) {
            qWarning() << "Error when trying to open desktop file with gio:" << gError->message;
            g_error_free(gError);
        }

        if (!ok) {
            qWarning() << "Failed to open desktop file with gio: g_app_info_launch_uris returns false";
        }
        g_object_unref(appInfo);
    };

    const auto reply = QDBusPendingReply<bool>(connection.asyncCall(msg));
    asyncWatcherFactory<bool>(
        reply,
        [fallbackOpenDesktopFile](QDBusPendingReply<bool> reply) {
            const auto ok = reply.argumentAt<0>();
            if (!ok) {
                fallbackOpenDesktopFile();
            }
        },
        [fallbackOpenDesktopFile](QDBusError UNUSED(error)) {
            fallbackOpenDesktopFile();
        }
    );
}

void Bridge::showAboutWindow() {
    if (!this->aboutWindow) {
        this->aboutWindow = new AboutWindow(this->getMainWindow());
        connect(this->aboutWindow, &QWidget::destroyed, [this](QObject* UNUSED(obj)) {
            this->aboutWindow = nullptr;
        });
        this->aboutWindow->setContent(this->aboutContent);
    }
    this->aboutWindow->show();
}

void Bridge::setAboutContent(const QString& html) {
    this->aboutContent = html;
    if (this->aboutWindow) {
        this->aboutWindow->setContent(this->aboutContent);
    }
}

unsigned int Bridge::layoutMargin() {
    return this->getMainWindow()->layout()->contentsMargins().left();
}

void Bridge::askWindowState() {
    const auto windowState = this->getMainWindow()->windowState();
    QTimer::singleShot(0, [this, windowState]() {
        emit this->windowStateAnswered(nameWindowState(windowState));
    });

}

void Bridge::askAppRegion() {
    if (!this->appRegion.isEmpty()) {
        QTimer::singleShot(0, [this]() {
            emit this->appRegionAnswered(this->appRegion);
        });
    }
}

void Bridge::calcTimezone() {
    if (this->timezone.isEmpty()) {
        QFile file("/etc/timezone");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Cannot open /etc/timezone for timezone information";
        }
        this->timezone = file.readAll().trimmed();
        emit this->timezoneAnswered(this->timezone);
    }
}

void Bridge::calcAppRegion() {
    assert(!this->timezone.isEmpty());
    if (this->timezone == "Asia/Shanghai" ||
        this->timezone == "Asia/Chongqing" ||
        this->timezone == "Asia/Chungking" ||
        this->timezone == "Asia/Urumqi" ||
        this->timezone == "Asia/Harbin" ||
        this->timezone == "Asia/PRC") {
        this->appRegion = "mainland";
    } else {
        this->appRegion = "international";
    }
    emit this->appRegionAnswered(this->appRegion);
}

void Bridge::askTimezone() {
    if (!this->timezone.isEmpty()) {
        QTimer::singleShot(0, [this]() {
            emit this->timezoneAnswered(this->timezone);
        });
    }
}

void Bridge::askLanguages() {
    if (this->languages.length()) {
        QTimer::singleShot(0, [this]() {
            emit this->languagesAnswered(this->languages);
        });
    }
}

void Bridge::calcLanguages() {
    const auto languages = g_get_language_names();
    const int length = sizeof(languages) / sizeof(void*);
    for (int i = 0; i < length + 1; i++) {
        this->languages << languages[i];
    }
    emit this->languagesAnswered(this->languages);
}