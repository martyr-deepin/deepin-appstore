/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <QApplication>
#include <QUrl>
class QCommandLineParser;
class QSettings;

class DBusInterface;
class MainWindow;

class ToolTip;

class Shell : public QApplication {
    Q_OBJECT
public:
    Shell(int& argc, char** argv);
    ~Shell();

    void showTooltip(const QString& text,
                     const QRect& globalGeometry);
    void setTooltipVisible(const bool& visible);

    QCommandLineParser* argsParser = nullptr;
    QString basePath;
    QSettings* settings = nullptr;
    QUrl initUrl;
    QString origin;

    void openManual();

private:
    ToolTip* tooltip = nullptr;
    DBusInterface* dbusInterface = nullptr;
    void parseOptions();
    void startWebView();
    MainWindow* win = nullptr;
};


#endif //SHELL_SHELL_H
