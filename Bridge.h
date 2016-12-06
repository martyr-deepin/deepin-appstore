/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SHELL_BRIDGE_H
#define SHELL_BRIDGE_H

#include <QObject>
#include <QStringList>
#include <QDBusConnection>
#include "LAStoreBridge.h"
class MainWindow;
class AboutWindow;
class DBusMenuManager;
class DBusMenu;

bool isProfessionalVersion();
// An object that bridges between the webpage and the QT world.
// Manages window behaviors, hot keys and other general behaviors.
class Bridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(LAStoreBridge* lastore
               MEMBER lastore
               CONSTANT
    )

public:
    Bridge(QObject* parent = nullptr);
    ~Bridge();

public Q_SLOTS:
    Q_INVOKABLE void exit();
    Q_INVOKABLE void showMinimize();
    Q_INVOKABLE void toggleMaximized();

    Q_INVOKABLE void showTooltip(const QString& text,
                                 const int& x, const int& y,
                                 const int& w, const int& h);
    Q_INVOKABLE void startMoving();
    Q_INVOKABLE void showMenu(QVariantMap content);

    Q_INVOKABLE void openExternalBrowser(const QString& url);
    Q_INVOKABLE void openDesktopFile(const QString& path);

    Q_INVOKABLE void setAboutContent(const QString& html);

    Q_INVOKABLE void askWindowState();
    Q_INVOKABLE void askLanguages();
    Q_INVOKABLE void askTimezone();
    Q_INVOKABLE void askAppRegion();


Q_SIGNALS:
    void loginRequested();
    void logoutRequested();

    void windowStateAnswered(QString);
    void languagesAnswered(QStringList);
    void timezoneAnswered(QString);
    void appRegionAnswered(QString);

private:
    void calcTimezone();
    QString timezone;

    void calcAppRegion();
    QString appRegion;

    void calcLanguages();
    QStringList languages;

    MainWindow* getMainWindow();
    AboutWindow* aboutWindow = nullptr;
    QString aboutContent;
    void showAboutWindow();

    // Menu
    DBusMenuManager* menuManager = nullptr;
    DBusMenu* menu = nullptr;
    void unregisterMenu();
    void onMenuUnregistered();
    QString menuPath;
    void onItemInvoked(const QString& id, bool checked);

    LAStoreBridge* lastore = nullptr;
    unsigned int layoutMargin();
};


#endif //SHELL_BRIDGE_H
