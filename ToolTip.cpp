/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include "ToolTip.h"

#include <QDebug>
#include <QLabel>
#include <QHBoxLayout>
//#include <QPropertyAnimation>


ToolTip::ToolTip(QWidget *parent) : QWidget(parent) {
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setFixedHeight(40);

    this->label = new QLabel(this);
    this->label->setAlignment(Qt::AlignCenter);

    this->layout = new QHBoxLayout(this);
    this->layout->addWidget(this->label);
    this->layout->setMargin(0);

//    this->animation = new QPropertyAnimation(this, "geometry");
//    this->animation->setEasingCurve(QEasingCurve::OutCubic);
//    this->animation->setDuration(201);

    this->setLayout(this->layout);
}

ToolTip::~ToolTip() {

}

void ToolTip::moveShow(const int x, const int y) {
//    if (!this->isVisible()) {
//        this->move(x, y);
//    } else {
//        this->animation->stop();
//        this->move(x, this->geometry().y());
//        this->animation->setStartValue(this->geometry());
//        this->animation->setEndValue(QRect(x, y, this->width(), this->height()));
//        this->animation->start();
//    }
    this->move(x, y);
    QWidget::show();
}

void ToolTip::updateStyle() {
    if (this->arrowDirection == ArrowLeft) {
        this->setStyleSheet("color: white;\
                             font-size: 12px;\
                             border-width: 6px 15px 6px 20px;\
                             padding-left: 6px;\
                             border-image: url(:/res/tooltip_left.png) 6 15 6 20 stretch;");
    } else {
        this->setStyleSheet("color: white;\
                             font-size: 12px;\
                             border-width: 6px 20px 6px 15px;\
                             border-image: url(:/res/tooltip_right.png) 6 20 6 15 stretch;");
    }
}

void ToolTip::show(const QString& text, const QRect& activationGeometry) {
    if (text.isEmpty()) {
        return this->hide();
    }
    this->label->setText(text);

    QFontMetrics metric(this->label->font());
    const auto width = metric.tightBoundingRect(text).width() + 40;
    this->setFixedWidth(width);
    if (activationGeometry.x() < width) {
        this->arrowDirection = ArrowDirection::ArrowLeft;
        this->updateStyle();
        this->moveShow(activationGeometry.x() + activationGeometry.width() + 5,
                       activationGeometry.center().y() - 14);
    } else {
        this->arrowDirection = ArrowDirection::ArrowRight;
        this->updateStyle();
        this->moveShow(activationGeometry.x() - width,
                       activationGeometry.center().y() - 14);
    }
}
