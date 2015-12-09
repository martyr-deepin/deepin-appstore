#include <QCommandLineParser>

#include "Shell.h"
#include "WebView.h"
#include "WebPage.h"

WebView::WebView(QWidget *parent) : QWebView(parent),
                                    borderRadius(4) {
    this->customPage = new WebPage(this);
    this->setPage(this->customPage);
    this->setAcceptDrops(false);

    const auto shell = static_cast<Shell*>(qApp);
    const auto settings = this->settings();
    settings->enablePersistentStorage(shell->basePath + "/storage");

    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, true);

    if (shell->argsParser->isSet("debug")) {
        settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    } else {
        this->setContextMenuPolicy(Qt::NoContextMenu);
    }

    connect(this, &QWebView::titleChanged, [this](const QString& title) {
        if (!title.isEmpty()) {
            const auto mainWin = this->parent();
            static_cast<QWidget * >(mainWin)->setWindowTitle(title);
            disconnect(this, &QWebView::titleChanged, nullptr, nullptr);
        }
    });
}

WebView::~WebView() {

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

void WebView::polish() {
    const auto region = QRegion(this->rect(), QRegion::RegionType::Rectangle);

    const auto tl = QRegion(0, 0, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
            QRegion(0, 0, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
    );
    const auto tr = QRegion(this->width() - borderRadius, 0, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
            QRegion(this->width() - 2 * borderRadius, 0, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
    );
    const auto bl = QRegion(0, this->height() - borderRadius, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
            QRegion(0, this->height() - 2 * borderRadius, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
    );
    const auto br = QRegion(this->width() - borderRadius, this->height() - borderRadius, borderRadius, borderRadius, QRegion::RegionType::Rectangle).subtracted(
            QRegion(this->width() - 2 * borderRadius, this->height() - 2 * borderRadius, borderRadius * 2, borderRadius * 2, QRegion::RegionType::Ellipse)
    );

    const auto result = region
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
