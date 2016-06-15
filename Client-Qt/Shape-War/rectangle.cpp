#include "rectangle.h"
#include <QPainter>
#include <QtMath>

Rectangle::Rectangle() {
    this->axis = 20;
    this->setPos(-50, -50);
    QVector<QPoint> shapePoint;
    double radian = qDegreesToRadians(360.0/4);
    for(int i=0; i<=4 ; ++i){
        shapePoint.append( QPoint( axis*qCos(radian*(i%4)), axis*qSin(radian*(i%4)) ) );
    }
    this->polygonShape = QPolygon(shapePoint);

    this->hpBar = new HpBar(1000, 2*axis, axis + 5);
}

QRectF Rectangle::boundingRect() const {
    qreal halfPenWidth = 1;
    return QRectF( -axis - halfPenWidth, -axis - halfPenWidth,
                  axis*2 + halfPenWidth, axis*2 + halfPenWidth);
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
    update();
}

void Rectangle::update() {
    this->hpBar->setHp(this->hp, this->maxHp);
    this->hpBar->setPos(this->x(), this->y());
    this->setRotation(this->angle);
}
