#ifndef PENTAGON_H
#define PENTAGON_H
#include <QGraphicsObject>
#include <QJsonObject>
#include <QPolygonF>

class Pentagon : public QGraphicsObject {
    Q_OBJECT
public:
    Pentagon();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void read(const QJsonObject &json);


private:
    int axis;
    qreal angle;
    QPolygonF polygonShape;
    int hp;
    int maxHp;
};
#endif // PENTAGON_H
