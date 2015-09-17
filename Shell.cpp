#include "Shell.h"
#include "MainWindow.h"


Shell::Shell(int &argc, char **argv) : QApplication(argc, argv) {
    this->parseOptions();
    MainWindow* win = new MainWindow();
    win->show();
    win->showLessImportant();
}

Shell::~Shell() {
    if (this->argsParser) {
        delete this->argsParser;
        this->argsParser = nullptr;
    }
}

#define COLLPASED_NAVITEM_WIDTH 48

void Shell::showTooltip(QString text, QPoint globalPos) {
    if (this->tooltip) {
        delete tooltip;
        tooltip = nullptr;
    }
    if (text.isEmpty()) {
        return;
    }
    this->tooltip = new DUI::DArrowRectangle(DUI::DArrowRectangle::ArrowRight);
    QLabel* content = new QLabel(text);
    content->setStyleSheet("QLabel {color: white}");
    QFont font("Arial", 12);
    content->setFont(font);

    QFontMetrics fm(font);
    auto width = fm.width(text);
    content->setFixedSize(width, fm.height());
    this->tooltip->setContent(content);
    this->tooltip->setArrowWidth(fm.height() + this->tooltip->margin());
    if (globalPos.x() <= width) {
        // show at right
        this->tooltip->setArrowDirection(DUI::DArrowRectangle::ArrowLeft);
        this->tooltip->show(globalPos.x() + COLLPASED_NAVITEM_WIDTH, globalPos.y());
    } else {
        // show at left
        this->tooltip->setArrowDirection(DUI::DArrowRectangle::ArrowRight);
        this->tooltip->show(globalPos.x(), globalPos.y());
    }
}

void Shell::setTooltipVisible(bool visible) {
    if (!this->tooltip) {
        return;
    }
    this->tooltip->setVisible(visible);
}

void Shell::parseOptions() {
    this->argsParser = new QCommandLineParser();
    this->argsParser->setApplicationDescription("Deepin Store");
    this->argsParser->addHelpOption();
    this->argsParser->addVersionOption();

    this->argsParser->addOptions({
        {{"d", "debug"},
         QCoreApplication::translate("main", "Enable debug mode.")},
        {"host",
         QCoreApplication::translate("main", "Override the default host with the specified one."),
         QCoreApplication::translate("main", "host")},
        {{"c", "clean"},
         QCoreApplication::translate("main", "Remove deepin-store related files under $HOME.")},
    });

    this->argsParser->process(qApp->arguments());
}
