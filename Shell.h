
#ifndef SHELL_SHELL_H
#define SHELL_SHELL_H

#include <QApplication>
#include <QCommandLineParser>
#include <QSettings>
#include <QPoint>
#include <libdui/darrowrectangle.h>
#include "DBusInterface.h"

class Shell : public QApplication {
    Q_OBJECT
public:
    Shell(int& argc, char** argv);
    ~Shell();

    void showTooltip(QString text, QPoint globalPos);
    void setTooltipVisible(bool visible);

    QCommandLineParser* argsParser = nullptr;
    QString basePath;
    QSettings* settings = nullptr;

private:
    DUI::DArrowRectangle* tooltip = nullptr;
    DBusInterface* dbusInterface = nullptr;
    void parseOptions();
};


#endif //SHELL_SHELL_H
