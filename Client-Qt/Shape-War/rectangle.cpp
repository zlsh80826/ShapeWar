#include "rectangle.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>

/*Rectangle::Rectangle() : Polygon() {
    this->constructPolygon();
}

void Rectangle::constructPolygon() {
    QVector<QPoint> shapePoint;
    double radian = qDegreesToRadians(360.0 / 4);
    for (int i = 0; i <= 4; ++i) {
        shapePoint.append(QPoint(axis * qCos(radian * (i % 4)),
                                 axis * qSin(radian * (i % 4))));
    }
    this->polygonShape = QPolygon(shapePoint);
}*/
