#include <QRegion>
#include <QCommandLineParser>

#include "Shell.h"
#include "WebView.h"
#include "WebPage.h"

WebView::WebView(QWidget *parent) : QWebView(parent), borderRadius(6) {
    customPage = new WebPage(this);
    this->setPage(customPage);
    this->setAcceptDrops(false);

    auto shell = static_cast<Shell*>(qApp);
    auto settings = this->settings();
    settings->enablePersistentStorage(shell->basePath + "/storage");

    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, true);

    if (shell->argsParser->isSet("debug")) {
        settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    } else {
        this->setContextMenuPolicy(Qt::NoContextMenu);
    }
}

WebView::~WebView() {
    if (customPage) {
        delete customPage;
        customPage = nullptr;
    }
}

void WebView::paintEvent(QPaintEvent* e) {
    QWebView::paintEvent(e);

    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath full;
    full.addRect(rect());

    QPainterPath path;
    path.addRoundedRect(rect(), borderRadius, borderRadius);
    painter.fillPath(full - path, QColor(Qt::black));
}
