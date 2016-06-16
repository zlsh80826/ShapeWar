#include "pentagon.h"
#include <QPainter>
#include <QtMath>

Pentagon::Pentagon() {
    this->axis = 25;
    QVector<QPoint> shapePoint;
    double radian = qDegreesToRadians(360.0 / 5);
    for (int i = 0; i <= 5; ++i) {
        shapePoint.append(QPoint(axis * qCos(radian * (i % 5)),
                                 axis * qSin(radian * (i % 5))));
    }
    this->polygonShape = QPolygon(shapePoint);
    this->hpBar = new HpBar(1000, axis * 2, axis + 5);
}

QRectF Pentagon::boundingRect() const {
    qreal halfPenWidth = 1 / 2;
    return QRectF(-axis - halfPenWidth, -axis - halfPenWidth,
                  axis * 2 + halfPenWidth, axis * 2 + halfPenWidth);
}

void Pentagon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) {

    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 255));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(118, 141, 252, 255), Qt::SolidPattern));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(this->shape());
}

QPainterPath Pentagon::shape() const {
    QPainterPath path;
    path.addPolygon(polygonShape);
    return path;
}

void Pentagon::read(const QJsonObject &json) {
    this->setVisible(json["visible"].toBool());
    this->hpBar->setVisible(json["visible"].toBool());
    this->setX(json["x"].toDouble());
    this->setY(json["y"].toDouble());
    this->hpBar->setPos(this->x(), this->y());
    this->hpBar->setHp(json["hp"].toInt(), json["maxHp"].toInt());
    this->angle = json["angle"].toDouble();
    this->setRotation(this->angle);
}
