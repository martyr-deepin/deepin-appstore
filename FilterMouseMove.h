/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SHELL_FILTERMOUSEMOVE_H
#define SHELL_FILTERMOUSEMOVE_H

#include <QObject>

class FilterMouseMove : public QObject {
    Q_OBJECT

public:
    explicit FilterMouseMove(QObject* object = nullptr);
    ~FilterMouseMove();

    bool eventFilter(QObject *obj, QEvent *event);
};


#endif //SHELL_FILTERMOUSEMOVE_H
