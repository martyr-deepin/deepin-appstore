/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#ifndef SHELL_TOOLTIP_H
#define SHELL_TOOLTIP_H

#include <QWidget>

class QLabel;
class QHBoxLayout;
//class QPropertyAnimation;


class ToolTip : public QWidget {
    Q_OBJECT

    Q_ENUMS(ArrowDirection)
public:
    enum ArrowDirection {
        ArrowLeft,
        ArrowRight,
    };

    explicit ToolTip(bool supportBorder, QWidget* parent = nullptr);
    ~ToolTip();

    void moveShow(const int x, const int y);

public Q_SLOTS:
    void show(const QString& text,
              const QRect& activationGeometry);


private:
    QLabel* label = nullptr;
    QHBoxLayout* layout = nullptr;
    ArrowDirection arrowDirection = ArrowRight;
    // QPropertyAnimation* animation = nullptr;

    const bool supportBorder = true;

    void updateStyle();
};


#endif //SHELL_TOOLTIP_H
