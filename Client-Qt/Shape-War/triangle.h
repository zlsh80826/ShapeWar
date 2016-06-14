#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <QGraphicsObject>
#include <QPolygonF>
#include <QJsonObject>

class Triangle : public QGraphicsObject {
    Q_OBJECT
public:
    Triangle();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void read(const QJsonObject &json);

private:
    int edge;
    qreal angle;
    QPolygonF polygonShape;
    int hp;
    int maxHp;
};

#endif // TRIANGLE_H
