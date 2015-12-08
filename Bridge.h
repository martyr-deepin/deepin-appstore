
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

// An object that bridges between the webpage and the QT world.
// Manages window behaviors, hot keys and other general behaviors.
class Bridge : public QObject {
    Q_OBJECT
    Q_PROPERTY(LAStoreBridge* lastore
               MEMBER lastore
    )

public:
    Bridge(QObject* parent = nullptr);
    ~Bridge();

public slots:
    Q_INVOKABLE void exit();
    Q_INVOKABLE void showMinimized();
    Q_INVOKABLE void toggleMaximized();

    Q_INVOKABLE QStringList getLocales();
    Q_INVOKABLE QString getAppRegion();
    Q_INVOKABLE QString getTimezoneName();
    Q_INVOKABLE void showTooltip(const QString& text,
                                 const int& x, const int& y,
                                 const int& w, const int& h);
    Q_INVOKABLE void startMoving();
    Q_INVOKABLE void showMenu(QString content);

    Q_INVOKABLE void openExternalBrowser(QString url);
    Q_INVOKABLE void openDesktopFile(QString path);

    Q_INVOKABLE void setAboutContent(QString html);

    Q_INVOKABLE void notifyCacheReady();

Q_SIGNALS:
    void loginRequested();
    void logoutRequested();
    void windowStateChanged(int);

private:
    QString timezoneName;
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
};


#endif //SHELL_BRIDGE_H
