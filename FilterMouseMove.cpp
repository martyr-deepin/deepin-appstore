/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/
#include <QEvent>
#include "Shell.h"
#include "FilterMouseMove.h"
#include "MainWindow.h"

FilterMouseMove::FilterMouseMove(QObject *object) : QObject(object) {

}

FilterMouseMove::~FilterMouseMove() {

}

bool FilterMouseMove::eventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::Leave: {
            static_cast<Shell*>(qApp)->setTooltipVisible(false);
            // fall through
        }
        case QEvent::Enter: {
            static_cast<Shell*>(qApp)->setTooltipVisible(true);
            const auto mainWindow = static_cast<MainWindow*>(this->parent());
            mainWindow->updateCursor(CornerEdge::Nil);
            // fall through
        }
        default: {
            // fall through
        }
    }
    return QObject::eventFilter(obj, event);
}
