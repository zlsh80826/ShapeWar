#include "rectangle.h"
#include <QPainter>

Rectangle::Rectangle() {
    this->edge = 20;
    QVector<QPoint> shapePoint;
    shapePoint.append(QPoint(-edge / 2, -edge / 2));
    shapePoint.append(QPoint(-edge / 2, edge / 2));
    shapePoint.append(QPoint(edge / 2, edge / 2));
    shapePoint.append(QPoint(edge / 2, -edge / 2));
    shapePoint.append(QPoint(-edge / 2, -edge / 2));
    this->polygonShape = QPolygon(shapePoint);
}

QRectF Rectangle::boundingRect() const {
    qreal halfPenWidth = 1;
    return QRectF(-edge / 2 - halfPenWidth, -edge / 2 - halfPenWidth,
                  edge + halfPenWidth, edge + halfPenWidth);
}

void Rectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                      QWidget *widget) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 255));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(255, 232, 105, 255), Qt::SolidPattern));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(this->shape());
}

QPainterPath Rectangle::shape() const {
    QPainterPath path;
    path.addPolygon(polygonShape);
    return path;
}

void Rectangle::read(const QJsonObject &json) {
    this->setX(json["x"].toDouble());
    this->setY(json["y"].toDouble());
    this->angle = json["angle"].toDouble();
    this->hp = json["hp"].toInt();
    this->maxHp = json["maxHp"].toInt();
}
