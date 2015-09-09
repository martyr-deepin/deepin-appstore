
#include "WebView.h"
#include "WebPage.h"

WebView::WebView(QWidget *parent) : QWebEngineView(parent) {
    auto customPage = new WebPage(this);
    this->setPage(customPage);
}

WebView::~WebView() {

}
