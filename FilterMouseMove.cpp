
#include <QEvent>
#include <QMouseEvent>
#include "FilterMouseMove.h"

FilterMouseMove::FilterMouseMove(QObject *object) : QObject(object) {

}

FilterMouseMove::~FilterMouseMove() {

}

bool FilterMouseMove::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Leave) {
        return true;
    }
    return QObject::eventFilter(obj, event);
}
