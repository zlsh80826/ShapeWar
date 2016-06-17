#include "bullet.h"
#include <QPainter>
#include <QDebug>

Bullet::Bullet() {
    this->radius = 20;
    this->disappearTimer = new QTimer(this);
    this->active = false;
    QObject::connect(this->disappearTimer, SIGNAL(timeout()), this, SLOT(decreaseOpacity()));
}

QRectF Bullet::boundingRect() const {
    qreal halfPenWidth = 1 / 2;
    return QRectF(-radius - halfPenWidth, -radius - halfPenWidth,
                  radius * 2 + halfPenWidth, radius * 2 + halfPenWidth);
}

void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) {
    QPen pen;
    pen.setWidth(3);
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
    if( (this->active == false) && (next_active==false) )
        return;
    this->setActive(next_active);
    this->setX(json["x"].toDouble());
    this->setY(json["y"].toDouble());
    this->radius = json["radius"].toInt();
}

void Bullet::setActive(bool visible) {
    //QGraphicsObject::setVisible(visible);
    if(visible == false) {
        this->active = false;
        this->disappear();
    } else {
        this->active = true;
        this->disappearTimer->stop();
        this->setOpacity(1);
    }
}

void Bullet::disappear() {
    this->disappearTimer->start(15);
}

void Bullet::decreaseOpacity() {
    this->setOpacity(this->opacity()-0.1);
    this->radius += 1;
}
