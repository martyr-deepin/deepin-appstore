
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
