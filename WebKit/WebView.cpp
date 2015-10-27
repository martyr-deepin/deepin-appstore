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
}

WebView::~WebView() {
    if (customPage) {
        delete customPage;
        customPage = nullptr;
    }
}
