#include "TextBrowser.h"

#include <QPainter>

TextBrowser::TextBrowser(QWidget* parent): QTextBrowser(parent),
                                           borderRadius(3) {

}

TextBrowser::~TextBrowser() {

}

void TextBrowser::paintEvent(QPaintEvent* event) {
    QTextBrowser::paintEvent(event);

    QPainter painter(this->viewport());
    painter.setCompositionMode(QPainter::CompositionMode_Clear);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QPainterPath full;
    full.addRect(rect());

    QPainterPath path;
    path.addRoundedRect(rect(), borderRadius, borderRadius);
    painter.fillPath(full - path, QColor(Qt::black));
}
