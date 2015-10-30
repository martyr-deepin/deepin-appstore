#include <QRegion>

#include "Shell.h"
#include "WebView.h"

WebView::WebView(QWidget *parent) : QWebView(parent) {
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
    this->polish();
}

WebView::~WebView() {
    if (customPage) {
        delete customPage;
        customPage = nullptr;
    }
}

void WebView::polish() {
    auto region = QRegion(this->rect(), QRegion::RegionType::Rectangle);
    auto tl = QRegion(0, 0, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
              QRegion(0, 0, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
    );
    auto tr = QRegion(this->width() - borderRadius, 0, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
              QRegion(this->width() - 2 * borderRadius, 0, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
    );
    auto bl = QRegion(0, this->height() - borderRadius, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
              QRegion(0, this->height() - 2 * borderRadius, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
    );
    auto br = QRegion(this->width() - borderRadius, this->height() - borderRadius, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
              QRegion(this->width() - 2 * borderRadius, this->height() - 2 * borderRadius, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
    );

    auto result = region
             .subtracted(tl)
             .subtracted(tr)
             .subtracted(bl)
             .subtracted(br);
    this->setMask(result);
}


void WebView::resizeEvent(QResizeEvent *event) {
    QWebView::resizeEvent(event);
    this->polish();
}
