#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QGraphicsObject>
#include <QJsonObject>
#include <QPolygonF>

class Rectangle : public QGraphicsObject {
    Q_OBJECT
public:
    Rectangle();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape() const;
    void read(const QJsonObject &json);

private:
    int edge;
    qreal angle;
    QPolygonF polygonShape;
    int hp;
    int maxHp;
};
#endif // RECTANGLE_H
