
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
    bool isInitialRun = true;

    void openManual();

signals:
    void applicationCacheFinished();

private:
    ToolTip* tooltip = nullptr;
    DBusInterface* dbusInterface = nullptr;
    void parseOptions();
    void onApplicationCacheFinished();
    void startWebView();
    MainWindow* win = nullptr;
};


#endif //SHELL_SHELL_H
