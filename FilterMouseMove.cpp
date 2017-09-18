/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
