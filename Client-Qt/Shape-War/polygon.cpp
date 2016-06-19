#include "polygon.h"
#include <QDebug>
#include <QPainter>
#include <QtMath>

Polygon::Polygon(int edgeCount) {
    this->axis = 20;
    this->hpBar = new HpBar(1000, 2 * axis, axis + 5);
    this->stage = INACTIVE;
    this->disappearTimer = new QTimer(this);
    this->reviveTimer = new QTimer(this);
    this->edgeCount = edgeCount;
    this->setOpacity(1);
    QObject::connect(this->disappearTimer, SIGNAL(timeout()), this,
                     SLOT(decreaseOpacity()));
    QObject::connect(this->reviveTimer, SIGNAL(timeout()), this,
                     SLOT(increaseOpacity()));
}

QRectF Polygon::boundingRect() const {
    qreal halfPenWidth = 3 / 2;
    return QRectF(-axis - halfPenWidth, -axis - halfPenWidth,
                  axis * 2 + halfPenWidth, axis * 2 + halfPenWidth);
}

void Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget) {
    (void)option;
    (void)widget;
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 255));
    painter->setPen(pen);
    if (edgeCount == 3)
        painter->setBrush(QBrush(QColor(252, 118, 119, 255), Qt::SolidPattern));
    else if (edgeCount == 4)
        painter->setBrush(QBrush(QColor(255, 232, 105, 255), Qt::SolidPattern));
    else
        painter->setBrush(QBrush(QColor(118, 141, 252, 255), Qt::SolidPattern));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(this->shape());
}

QPainterPath Polygon::shape() const {
    QPainterPath path;
    path.addPolygon(polygonShape);
    return path;
}

void Polygon::read(const QJsonObject &json) {
    bool next_active = json["visible"].toBool();
    if ((this->stage != ACTIVE) && (next_active == false) || (this->stage==DISAPPEARING) )
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

void Polygon::setStage(bool control) {
    if (control == false) {
        this->stage = DISAPPEARING;
        this->disappear();
    } else {
        if (this->stage == ACTIVE)
            return;
        this->stage = ACTIVE;
        this->constructPolygon();
        this->reviveTimer->start(20);
    }
}

void Polygon::disappear() {
    this->disappearTimer->start(20);
    this->hpBar->setVisible(false);
}

void Polygon::decreaseOpacity() {
    if (this->opacity() <= 0.05) {
        this->setOpacity(0);
        this->disappearTimer->stop();
        this->stage = INACTIVE;
        return;
    }
    this->setOpacity(this->opacity() - 0.08);
    this->axis += 1;
    this->constructPolygon();
}

void Polygon::increaseOpacity() {
    if (this->opacity() >= 0.95) {
        this->setOpacity(1);
        this->reviveTimer->stop();
        this->hpBar->setVisible(true);
        return;
    }
    this->setOpacity(this->opacity() + 0.08);
}

void Polygon::constructPolygon() {
    QVector<QPoint> shapePoint;
    double radian = qDegreesToRadians(360.0 / edgeCount);
    for (int i = 0; i <= edgeCount; ++i) {
        shapePoint.append(QPoint(axis * qCos(radian * (i % edgeCount)),
                                 axis * qSin(radian * (i % edgeCount))));
    }
    this->polygonShape = QPolygon(shapePoint);
}
