#include "bullet.h"
#include <QDebug>
#include <QPainter>

Bullet::Bullet(int) {
    this->setOpacity(0);
    this->radius = 20;
    this->disappearTimer = new QTimer(this);
    this->stage = INACTIVE;
    QObject::connect(this->disappearTimer, SIGNAL(timeout()), this,
                     SLOT(decreaseOpacity()));
}

QRectF Bullet::boundingRect() const {
    qreal halfPenWidth = 2;
    return QRectF(-radius - halfPenWidth, -radius - halfPenWidth,
                  radius * 2 + halfPenWidth, radius * 2 + halfPenWidth);
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) {
    (void)option;
    (void)widget;
    QPen pen;
    pen.setWidth(4);
    pen.setColor(QColor(85, 85, 85, 255));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(241, 78, 84, 255), Qt::SolidPattern));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(this->shape());
}

QPainterPath Bullet::shape() const {
    QPainterPath path;
    path.addEllipse(0, 0, radius, radius);
    return path;
}

void Bullet::read(const QJsonObject &json) {
    bool next_active = json["visible"].toBool();
    if ( ((this->stage != ACTIVE) && (next_active == false)) || (this->stage==DISAPPEARING) )
        return;
    this->setStage(next_active);
    this->setX(json["x"].toDouble());
    this->setY(json["y"].toDouble());
    this->radius = json["radius"].toInt();
}

void Bullet::setStage(bool control) {
    if (control == false) {
        this->stage = DISAPPEARING;
        this->disappear();
    } else {
        this->stage = ACTIVE;
        this->setOpacity(1);
    }
}

void Bullet::disappear() {
    this->disappearTimer->start(15);
}

void Bullet::decreaseOpacity() {
    if (this->opacity() <= 0.05) {
        this->setOpacity(0);
        this->disappearTimer->stop();
        this->stage = INACTIVE;
    }
    this->setOpacity(this->opacity() - 0.08);
    this->radius += 1;
}
