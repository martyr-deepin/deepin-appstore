
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
#include <QVariantList>

#include <QProcess>
#include <QDesktopServices>
#include "MainWindow.h"


Bridge::Bridge(QObject *parent) : QObject(parent) {
    this->lastore = new LAStoreBridge(this);
    this->menuManager = new DBusMenuManager(this);
    this->registerMenu();

    // bind window state change
    auto mainWin = this->getMainWindow();
    connect(mainWin, &MainWindow::windowStateChanged,
            this, [this](Qt::WindowState state) {
                emit this->windowStateChanged(state);
            });
}

Bridge::~Bridge() {
    if (this->m_menu) {
        delete this->m_menu;
        this->m_menu = nullptr;
    }
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

void Bridge::showTooltip(QString text, int x, int y) {
    MainWindow* mainWindow = this->getMainWindow();
    QPoint globalPos = mainWindow->mapToGlobal(QPoint(x, y));
    static_cast<Shell*>(qApp)->showTooltip(text, globalPos);
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

void Bridge::startMoving(int x, int y) {
    this->getMainWindow()->startMoving(x, y);
}


MainWindow* Bridge::getMainWindow() {
    WebPage* webPage = static_cast<WebPage*>(this->parent());
    WebView* webView = static_cast<WebView*>(webPage->parent());
    MainWindow* mainWindow = static_cast<MainWindow*>(webView->window());
    return mainWindow;
}

// Window Menu
void Bridge::registerMenu() {
    auto pendingReply = this->menuManager->RegisterMenu();
    pendingReply.waitForFinished();
    if (pendingReply.isValid()) {
        QDBusObjectPath path = pendingReply.reply().arguments()[0].value<QDBusObjectPath>();
        QString pathStr = path.path();
        if (this->m_menu) {
            delete this->m_menu;
            this->m_menu = nullptr;
        }
        this->m_menu = new DBusMenu(pathStr, this);
        connect(this->m_menu, &DBusMenu::MenuUnregistered,
                this, &Bridge::onMenuUnregistered);

        connect(this->m_menu, &DBusMenu::ItemInvoked,
                this, &Bridge::onItemInvoked);
    } else {
        qDebug() << pendingReply.error().message();
    }
}

void Bridge::onMenuUnregistered() {
    qDebug() << "menu unregistered";
    this->registerMenu();
}

void Bridge::showMenu(QString content) {
    this->m_menu->ShowMenu(content);
}

void Bridge::onItemInvoked(const QString & id, bool checked) {
    if (id == "exit") {
        qApp->exit(0);
    } else if (id == "help") {
        QString program = "/usr/bin/dman";
        QStringList args;
        args << "deepin-store";
        auto dManual = new QProcess(this);
        dManual->startDetached(program, args);
    } else if (id == "about") {

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
    auto stdPath = path.toStdString();
    const char* cPath = stdPath.c_str();
    GDesktopAppInfo* appInfo = g_desktop_app_info_new_from_filename(cPath);
    g_app_info_launch_uris(reinterpret_cast<GAppInfo*>(appInfo), NULL, NULL, NULL);
    g_object_unref(appInfo);
}
