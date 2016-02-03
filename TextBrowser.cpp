/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
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
