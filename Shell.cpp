
#include "Shell.h"
#include "MainWindow.h"

Shell::Shell(int &argc, char **argv) : QApplication(argc, argv) {
    MainWindow* win = new MainWindow();
    win->show();
    win->showLessImportant();
}

Shell::~Shell() {

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
