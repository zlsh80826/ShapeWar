#include "rectangle.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>

Rectangle::Rectangle() {
    this->axis = 20;
    this->constructPolygon();
    this->hpBar = new HpBar(1000, 2 * axis, axis + 5);
    this->stage=INACTIVE;
    this->disappearTimer = new QTimer(this);
    this->reviveTimer = new QTimer(this);
    QObject::connect(this->disappearTimer, SIGNAL(timeout()), this, SLOT(decreaseOpacity()));
    QObject::connect(this->reviveTimer, SIGNAL(timeout()), this, SLOT(increaseOpacity()));
}

QRectF Rectangle::boundingRect() const {
    qreal halfPenWidth = 3/2;
    return QRectF(-axis - halfPenWidth, -axis - halfPenWidth,
                  axis * 2 + halfPenWidth, axis * 2 + halfPenWidth);
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
    this->hpBar->setOpacity(this->opacity());
    bool next_active = json["visible"].toBool();
    if( (this->stage != ACTIVE) && (next_active==false) )
        return;
    this->setX(json["x"].toDouble());
    this->setY(json["y"].toDouble());
    this->axis = json["radius"].toInt();
    this->setStage(next_active);
    this->angle = json["angle"].toDouble();
    this->setRotation(this->angle);
    this->hpBar->setHp(json["hp"].toInt(), json["maxHp"].toInt());
    this->hpBar->setPos(this->x(), this->y());
}

void Rectangle::setStage(bool control) {
    if(control == false) {
        this->stage = DISAPPEARING;
        this->disappear();
    } else {
        if(this->stage==ACTIVE)
            return ;
        this->stage = ACTIVE;
        this->constructPolygon();
        this->reviveTimer->start(20);
    }
}

void Rectangle::disappear() {
    this->disappearTimer->start(20);
}

void Rectangle::decreaseOpacity() {
    if(this->opacity() == 0){
        this->disappearTimer->stop();
        this->stage = INACTIVE;
        return ;
    }
    this->setOpacity(this->opacity()-0.08);
    this->axis += 1;
    this->constructPolygon();
}

void Rectangle::increaseOpacity() {
    if(this->opacity() == 1){
        this->reviveTimer->stop();
        return ;
    }
    this->setOpacity(this->opacity()+0.08);
}

void Rectangle::constructPolygon() {
    QVector<QPoint> shapePoint;
    double radian = qDegreesToRadians(360.0 / 4);
    for (int i = 0; i <= 4; ++i) {
        shapePoint.append(QPoint(axis * qCos(radian * (i % 4)),
                                 axis * qSin(radian * (i % 4))));
    }
    this->polygonShape = QPolygon(shapePoint);
}
