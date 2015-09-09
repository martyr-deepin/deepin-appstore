
#include "WebView.h"
#include "WebPage.h"

WebView::WebView(QWidget *parent) : QWebView(parent) {
    customPage = new WebPage(this);
    this->setAcceptDrops(false);
    this->setPage(customPage);

    this->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    this->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    this->settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    this->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);

    this->settings()->enablePersistentStorage(QString("/tmp"));
    // TODO: enable this, when not debugging
    // this->setContextMenuPolicy(Qt::NoContextMenu);
}

WebView::~WebView() {
    if (customPage) {
        delete customPage;
        customPage = nullptr;
    }
}
