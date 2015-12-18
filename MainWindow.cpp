#include "common.h"
#include <QDebug>
#include <QIcon>
#include <QLayout>
#include <QEvent>
#include <QKeyEvent>

#include "Shell.h"
#include "MainWindow.h"
#include "FilterMouseMove.h"

MainWindow::MainWindow(StupidWindow *parent) : StupidWindow(parent) {
    qDebug() << "Build with" << WebWidgetName;
    this->setWindowIcon(QIcon::fromTheme("deepin-appstore"));
    this->resize(1028, 700);
    this->setMinimumSize(906, 680);
    this->setMouseTracking(true);
    this->setAttribute(Qt::WA_QuitOnClose, true);
    this->setAttribute(Qt::WA_DeleteOnClose, true);

    this->webView = new WebView(this);

    // Leave event will cause problems with <horizontal-resizer>, eat leave events!
    const auto filter = new FilterMouseMove(this);
    this->webView->installEventFilter(filter);

    connect(this->webView, &WebView::titleChanged, [this](const QString& title) {
        if (!title.isEmpty()) {
            this->setWindowTitle(title);
//            disconnect(this->webView, &WebView::titleChanged, nullptr, nullptr);
        }
    });

    this->layout()->addWidget(this->webView);
}

void MainWindow::toggleMaximized() {
    if (this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}

void MainWindow::changeEvent(QEvent *event) {
    StupidWindow::changeEvent(event);
    if (event->type() == QEvent::WindowStateChange) {
        emit this->windowStateChanged((Qt::WindowState)(int)this->windowState());
    }
}

void MainWindow::setUrl(const QUrl &url) {
    this->webView->setUrl(url);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_F1 &&
        event->modifiers() == Qt::NoModifier) {
        const auto shell = static_cast<Shell*>(qApp);
        shell->openManual();
    };
}

MainWindow::~MainWindow() {

}
