#include <QDebug>
#include <QPainter>
#include <QTextStream>

#include "ProgressButton.h"


ProgressButton::ProgressButton(QWidget* parent) : QWidget(parent) {

}

ProgressButton::~ProgressButton() {
    if (this->painter) {
        delete painter;
    }
}

void ProgressButton::setError(bool errored) {
    this->errored = errored;
}

void ProgressButton::setProgress(double progress) {
    if (this->progress != progress) {
        this->progress = progress;
        emit this->needRepaint();
    }
}

void ProgressButton::setBody(ProgressBody type) {
    this->bodyType = type;
}


void ProgressButton::paintEvent(QPaintEvent* event) {
    if (!this->painter) {
        painter = new QPainter();
    }
    painter->begin(this);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBackgroundMode(Qt::TransparentMode);

    // empty the canvas
    painter->setCompositionMode(QPainter::CompositionMode_Source);
    painter->fillRect(0, 0, this->width(), this->height(), Qt::transparent);
    painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    // draw ring background
    auto circlePen = QPen(this->ringBackgroundColor);
    circlePen.setWidth(this->penWidth);
    painter->setPen(circlePen);
    painter->drawArc(QRectF(QPointF(margin, margin),
                            QPointF(this->width() - margin, this->height() - margin)),
                     0 * 16, 360 * 16);

    // draw ring foreground
    auto finishedPen = QPen(this->errored ? this->ringErroredForegroundColor : this->ringForegroundColor);
    finishedPen.setWidth(this->penWidth);
    painter->setPen(finishedPen);
    painter->drawArc(QRectF(QPointF(margin, margin),
                            QPointF(this->width() - margin, this->height() - margin)),
                     90 * 16, -(this->progress * 360) * 16);

    // draw body
    switch (this->bodyType) {
        case ProgressBody::Percentage: {
            QString imgSrc;
            if (this->isHover) {
                if (this->state == "failed" || this->state == "pause") {
                    imgSrc = ":/res/start.svg";
                } else if (this->state == "running"){
                    imgSrc = ":/res/pause.svg";
                } else if (this->state == "success") {
                    // nothing
                } else {
                    qDebug() << "Unknown Progress State:" << this->state;
                }
                if (imgSrc.count()) {
                    painter->drawImage(QRectF(this->width() / 2 - 8,
                                              this->height() / 2 - 8, 16, 16),
                                       QImage(imgSrc));
                }
            }
            if (!imgSrc.count()) {
                auto percentagePen = QPen(this->bodyPercentageColor);
                painter->setPen(percentagePen);
                QString bodyText;
                QTextStream t(&bodyText);
                t.setRealNumberPrecision(0);
                t.setRealNumberNotation(QTextStream::FixedNotation);
                t << this->progress * 100.;
                QFont f("Arial", 10);
                painter->setFont(f);
                painter->drawText(0, 0,
                                  this->width(), this->height(), Qt::AlignCenter,
                                  bodyText);
            }
            break;
        }
        case ProgressBody::None: {
            break;
        }
        default: {
            qDebug() << "Undefined body type";
        }
    }
    painter->end();
}

void ProgressButton::enterEvent(QEvent *qEvent) {
    qDebug() << "ProgressButton::enterEvent received";
    if (!this->isHover) {
        this->isHover = true;
        emit this->needRepaint();
    }
}

void ProgressButton::leaveEvent(QEvent *qEvent) {
    qDebug() << "ProgressButton::leaveEvent received";
    if (this->isHover) {
        this->isHover = false;
        emit this->needRepaint();
    }

}

void ProgressButton::setState(QString state) {
    this->state = state;
}
