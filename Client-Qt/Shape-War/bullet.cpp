#include "bullet.h"
#include <QPainter>

Bullet::Bullet() {
    this->radius = 20;
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
    this->setX(json["x"].toDouble());
    this->setY(json["y"].toDouble());
}
