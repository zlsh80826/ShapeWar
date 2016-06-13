#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <QGraphicsObject>
#include <QPolygonF>

class Triangle : public QGraphicsObject {
    Q_OBJECT
public:
    Triangle();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void setTargetAngle(qreal targetAngle);


private:
    int edge;
    qreal targetAngle;
    QPolygonF polygonShape;
};

#endif // TRIANGLE_H
