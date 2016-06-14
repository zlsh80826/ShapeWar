#include "triangle.h"
#include <QPainter>

Triangle::Triangle() {
    this->edge = 20;
    QVector<QPoint> shapePoint;
    shapePoint.append(QPoint(0, -(edge * 2) / 3));
    shapePoint.append(QPoint(-edge / 2, edge / 3));
    shapePoint.append(QPoint(edge / 2, edge / 3));
    shapePoint.append(QPoint(0, -(edge * 2) / 3));
    this->polygonShape = QPolygon(shapePoint);
}

QRectF Triangle::boundingRect() const {
    qreal halfPenWidth = 1;
    return QRectF(-edge / 2 - halfPenWidth, (-edge * 2) / 3 - halfPenWidth,
                  edge + halfPenWidth, edge + halfPenWidth);
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
    this->hp = json["hp"].toInt();
    this->maxHp = json["maxHp"].toInt();
}
