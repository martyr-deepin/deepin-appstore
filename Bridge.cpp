
#include <QApplication>

// for tooltips
#include <QFont>
#include <QFontMetrics>
#include <QLabel>

// for locales
#include <glib.h>
#include <glib/gi18n.h>


#include "Bridge.h"
#include "WebWidget.h"
#include <QVariantList>


#define COLLPASED_NAVITEM_WIDTH 48


Bridge::Bridge(QObject *parent) : QObject(parent) {
    auto sessionBus = QDBusConnection::sessionBus();
    this->lastore = new LAStoreBridge(this);
    sessionBus.registerService("org.deepin.appstore.shell");
    sessionBus.registerObject("/", this,
                              QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllProperties | QDBusConnection::ExportAllSignals);

    this->registerMenu();
}

Bridge::~Bridge() {
    if (this->m_menu) {
        delete this->m_menu;
    }
    if (this->lastore) {
        delete this->lastore;
    }
}

void Bridge::exit() {
    qApp->exit();
}

void Bridge::maximize() {

}

void Bridge::minimize() {

}

//void Bridge::windowStateChanged(Qt::WindowState state) {
//
//}

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
    if (activeTooltip) {
        delete activeTooltip;
        activeTooltip = nullptr;
    }
    if (text.isEmpty()) {
        return;
    }
    activeTooltip = new DUI::DArrowRectangle(DUI::DArrowRectangle::ArrowRight);
    QLabel* content = new QLabel(text);
    content->setStyleSheet("QLabel {color: white}");
    QFont font("Arial", 12);
    content->setFont(font);

    MainWindow* mainWindow = this->getMainWindow();
    QPoint globalPos = mainWindow->mapToGlobal(QPoint(x, y));

    QFontMetrics fm(font);
    auto width = fm.width(text);
    content->setFixedSize(width, fm.height());
    activeTooltip->setContent(content);
    activeTooltip->setArrowWidth(fm.height() + activeTooltip->margin());
    if (globalPos.x() <= width) {
        // show at right
        activeTooltip->setArrorDirection(DUI::DArrowRectangle::ArrowLeft);
        activeTooltip->show(globalPos.x() + COLLPASED_NAVITEM_WIDTH, globalPos.y());
    } else {
        // show at left
        activeTooltip->setArrorDirection(DUI::DArrowRectangle::ArrowRight);
        activeTooltip->show(globalPos.x(), globalPos.y());
    }
}

QString Bridge::getAppRegion() {
    QString tz = this->getTimezoneName();
    if (tz == "Asia/Shanghai" ||
        tz == "Asia/Chongqing" ||
        tz == "Asia/Chungking" ||
        tz == "Asia/Urumqi" ||
        tz == "Asia/Harbin" ||
        tz == "Asia/PRC") {
        return QString("ChinaMainland");
    }
    return QString("International");
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

void Bridge::toggleMaximized() {
    this->getMainWindow()->toggleMaximized();
}

MainWindow* Bridge::getMainWindow() {
    WebPage* webPage = static_cast<WebPage*>(this->parent());
    WebView* webView = static_cast<WebView*>(webPage->parent());
    MainWindow* mainWindow = static_cast<MainWindow*>(webView->window());
    return mainWindow;
}

// Window Menu
void Bridge::registerMenu() {
    auto dbusMenuManager = new DBusMenuManager(this);
    auto pendingReply = dbusMenuManager->RegisterMenu();
    pendingReply.waitForFinished();
    if (pendingReply.isValid()) {
        QDBusObjectPath path = pendingReply.reply().arguments()[0].value<QDBusObjectPath>();
        QString pathStr = path.path();
        if (this->m_menu) {
            delete this->m_menu;
        }
        this->m_menu = new DBusMenu(pathStr, this);
        connect(this->m_menu, &DBusMenu::MenuUnregistered,
                this, &Bridge::onMenuUnregistered);
    }
}

void Bridge::onMenuUnregistered() {
    qDebug() << "menu unregistered";
    this->registerMenu();
}

void Bridge::showMenu(QString content) {
    this->m_menu->ShowMenu(content);
}
