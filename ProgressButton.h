#ifndef SHELL_PROGRESSBUTTON_H
#define SHELL_PROGRESSBUTTON_H

#include <QWidget>
class QPainter;

enum ProgressBody {
    None,
    Percentage,
};

class ProgressButton : public QWidget {
    Q_OBJECT
public:
    explicit ProgressButton(QWidget* parent = nullptr);
    ~ProgressButton();

    void setError(bool errored);
    void setProgress(double progress);
    void setState(QString state);
    void setBody(ProgressBody type);

signals:
    void needRepaint();

private:
    QPainter* painter = nullptr;
    void paintEvent(QPaintEvent *);
    void enterEvent(QEvent* qEvent);
    void leaveEvent(QEvent* qEvent);

    float margin = 2.0;
    float penWidth = 3.0;

    QColor ringBackgroundColor = QColor(0, 0, 0, (int)(255 * 0.2));
    QColor ringForegroundColor = QColor(0, 162, 255, 255);
    QColor ringErroredForegroundColor = QColor(0xff, 0x8f, 0x00, 0xff);
    QColor bodyPercentageColor = Qt::black;

    ProgressBody bodyType = ProgressBody::None;

    double progress = 0.0;
    bool isHover = false;
    QString state;
    bool errored = false;
};


#endif //SHELL_PROGRESSBUTTON_H
