#include "common.h"
#define QT_NO_KEYWORDS
    // for opening .desktop files
    #include <gio/gio.h>
    #include <gio/gdesktopappinfo.h>

    // for locales
    #include <glib.h>
    #include <glib/gi18n.h>
#undef QT_NO_KEYWORDS

#include <QApplication>

// for tooltips
#include <QFont>
#include <QFontMetrics>
#include <QLabel>

#include "Shell.h"
#include "Bridge.h"
#include "WebWidget.h"

#include <QDesktopServices>
#include "MainWindow.h"
#include "AboutWindow.h"
#include "dbusmenu.h"
#include "dbusmenumanager.h"

Bridge::Bridge(QObject *parent) : QObject(parent) {
    this->lastore = new LAStoreBridge(this);
    this->menuManager = new DBusMenuManager(this);

    // bind window state change
    auto mainWin = this->getMainWindow();
    connect(mainWin, &MainWindow::windowStateChanged,
            this, [this](Qt::WindowState state) {
                emit this->windowStateChanged((int)state);
            });
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

void Bridge::showMinimized() {
    this->getMainWindow()->showMinimized();
}

void Bridge::toggleMaximized() {
    this->getMainWindow()->toggleMaximized();
}

QStringList Bridge::getLocales() {
    QStringList result;
    auto languageNames = g_get_language_names();
    int length = sizeof(languageNames) / sizeof(void*);
    for (int i = 0; i < length + 1; i++) {
        result << languageNames[i];
    }

    return result;
}

void Bridge::showTooltip(const QString& text,
                         const int& x, const int& y,
                         const int& w, const int& h) {
    const auto mainWindow = this->getMainWindow();
    const auto globalPos = mainWindow->mapToGlobal(QPoint(x, y));
    static_cast<Shell*>(qApp)->showTooltip(text,
                                           QRect(globalPos.x(), globalPos.y(), w, h));
}

QString Bridge::getAppRegion() {
    QString tz = this->getTimezoneName();
    if (tz == "Asia/Shanghai" ||
        tz == "Asia/Chongqing" ||
        tz == "Asia/Chungking" ||
        tz == "Asia/Urumqi" ||
        tz == "Asia/Harbin" ||
        tz == "Asia/PRC") {
        return QString("mainland");
    }
    return QString("international");
}

QString Bridge::getTimezoneName() {
    if (timezoneName.isEmpty()) {
        QFile file("/etc/timezone");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            timezoneName = "";
        }
        timezoneName = file.readAll().trimmed();
    }
    return timezoneName;
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

void Bridge::showMenu(QString content) {
    if (this->menuPath.size()) {
        qWarning() << "Another menu is active";
        return;
    }
    const auto pendingReply = this->menuManager->RegisterMenu();
    const auto watcher = new QDBusPendingCallWatcher(pendingReply, this);
    connect(watcher, &QDBusPendingCallWatcher::finished,
            [this, watcher, content](QDBusPendingCallWatcher* call) {
                QDBusPendingReply<QDBusObjectPath> reply = *call;
                if (reply.isError()) {
                    auto error = reply.error();
                    qWarning() << error.name() << error.message();
                } else {
                    this->menuPath = reply.argumentAt<0>().path();
                    this->menu = new DBusMenu(this->menuPath, this);
                    connect(this->menu, &DBusMenu::MenuUnregistered,
                            this, &Bridge::onMenuUnregistered);

                    connect(this->menu, &DBusMenu::ItemInvoked,
                            this, &Bridge::onItemInvoked);
                    this->menu->ShowMenu(content);
                }

                delete watcher;
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

void Bridge::openExternalBrowser(QString url) {
    QDesktopServices::openUrl(QUrl(url));
}

void Bridge::openDesktopFile(QString path) {
    if (path == "") {
         return;
    }
    auto connection = QDBusConnection::sessionBus();
    auto msg = QDBusMessage::createMethodCall("com.deepin.SessionManager",
                                              "/com/deepin/StartManager",
                                              "com.deepin.StartManager",
                                              "Launch");
    msg << path;

    const auto call = connection.asyncCall(msg);
    QDBusPendingReply<> reply(call);
    reply.waitForFinished();
    if (reply.isError()) {
        qDebug() << reply.error();
        // use fallback method to call
        auto stdPath = path.toStdString();
        const char* cPath = stdPath.c_str();
        GDesktopAppInfo* appInfo = g_desktop_app_info_new_from_filename(cPath);
        g_app_info_launch_uris(reinterpret_cast<GAppInfo*>(appInfo), NULL, NULL, NULL);
        g_object_unref(appInfo);
    }
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

void Bridge::setAboutContent(QString html) {
    this->aboutContent = html;
    if (this->aboutWindow) {
        this->aboutWindow->setContent(this->aboutContent);
    }
}

void Bridge::notifyCacheReady() {
    const auto shell = static_cast<Shell*>(qApp);
    emit shell->applicationCacheFinished();
}
