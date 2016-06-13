#ifndef PENTAGON_H
#define PENTAGON_H
#include <QGraphicsObject>
#include <QPolygonF>

class Pentagon : public QGraphicsObject {
    Q_OBJECT
public:
    Pentagon();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void setTargetAngle(qreal targetAngle);


private:
    int axis;
    qreal targetAngle;
    QPolygonF polygonShape;
};
#endif // PENTAGON_H
