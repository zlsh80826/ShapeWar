#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QGraphicsObject>
#include <QPolygonF>

class Rectangle : public QGraphicsObject {
    Q_OBJECT
public:
    Rectangle();
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
#endif // RECTANGLE_H
