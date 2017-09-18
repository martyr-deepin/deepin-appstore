/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <DApplication>
#if defined(qApp)
#undef qApp
#endif
#define qApp (static_cast<QApplication *>(QCoreApplication::instance()))

#include <QUrl>
class QCommandLineParser;
class QSettings;

class DBusInterface;
class MainWindow;

class ToolTip;

class Shell : public Dtk::Widget::DApplication {
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
