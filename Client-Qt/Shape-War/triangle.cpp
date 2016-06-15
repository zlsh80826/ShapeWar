#include "triangle.h"
#include <QPainter>
#include <QtMath>

Triangle::Triangle() {
    this->axis = 20;
    this->setPos(-50, -50);
    QVector<QPoint> shapePoint;
    for (int i = 0; i <= 3; ++i) {
        double radian = qDegreesToRadians(360.0 / 3);
        shapePoint.append(QPoint(axis * qCos(radian * (i % 3)),
                                 axis * qSin(radian * (i % 3))));
    }
    this->polygonShape = QPolygon(shapePoint);
    this->hpBar = new HpBar(1000, 2 * axis, axis + 5);
}

QRectF Triangle::boundingRect() const {
    qreal halfPenWidth = 1;
    return QRectF(-axis - halfPenWidth, -axis - halfPenWidth,
                  axis * 2 + halfPenWidth, axis * 2 + halfPenWidth);
}

void Triangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 255));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(252, 118, 119, 255), Qt::SolidPattern));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(this->shape());
}

QPainterPath Triangle::shape() const {
    QPainterPath path;
    path.addPolygon(polygonShape);
    return path;
}

void Triangle::read(const QJsonObject &json) {
    this->setX(json["x"].toDouble());
    this->setY(json["y"].toDouble());
    this->angle = json["angle"].toDouble();
    this->setRotation(angle);
    this->hpBar->setPos(this->x(), this->y());
    this->hpBar->setHp(json["hp"].toInt(), json["maxHp"].toInt());
}
