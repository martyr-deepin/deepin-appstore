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

    explicit ToolTip(QWidget* parent = nullptr);
    ~ToolTip();

    void moveShow(const int x, const int y);

public slots:
    void show(const QString& text,
              const QRect& activationGeometry);


private:
    QLabel* label = nullptr;
    QHBoxLayout* layout = nullptr;
    ArrowDirection arrowDirection = ArrowRight;
    // QPropertyAnimation* animation = nullptr;

    void updateStyle();
};


#endif //SHELL_TOOLTIP_H
