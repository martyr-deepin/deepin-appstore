
#ifndef SHELL_BRIDGE_H
#define SHELL_BRIDGE_H

#include <QObject>
#include <QStringList>
#include <QDBusConnection>
#include <libdui/darrowrectangle.h>
#include "MainWindow.h"

#include "dbusmenu.h"
#include "dbusmenumanager.h"
#include "LAStoreBridge.h"


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
    Q_INVOKABLE void maximize();
    Q_INVOKABLE void minimize();

    Q_INVOKABLE QStringList getLocales();
    Q_INVOKABLE QString getAppRegion();
    Q_INVOKABLE QString getTimezoneName();
    Q_INVOKABLE void showTooltip(QString text, int x, int y);

    Q_INVOKABLE void startMoving(int x, int y);
    Q_INVOKABLE void toggleMaximized();
    Q_INVOKABLE void showMenu(QString content);

    Q_INVOKABLE void openExternalBrowser(QString url);
    Q_INVOKABLE void openDesktopFile(QString path);


Q_SIGNALS:
    void loginRequested();
    void logoutRequested();
//    void windowStateChanged(Qt::WindowState);

private:
    DUI::DArrowRectangle* activeTooltip = nullptr;
    QString timezoneName;
    MainWindow* getMainWindow();


    // Menu
    DBusMenuManager* menuManager = nullptr;
    DBusMenu* m_menu = nullptr;
    void onItemInvoked(const QString & id, bool checked);
    void registerMenu();
    void onMenuUnregistered();

    LAStoreBridge* lastore = nullptr;
};


#endif //SHELL_BRIDGE_H
