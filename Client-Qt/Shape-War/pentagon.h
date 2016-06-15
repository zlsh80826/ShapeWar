#ifndef PENTAGON_H
#define PENTAGON_H
#include <QGraphicsObject>
#include <QJsonObject>
#include <QPolygonF>
#include <hpbar.h>

class Pentagon : public QGraphicsObject {
    Q_OBJECT
public:
    Pentagon();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape() const;
    void read(const QJsonObject &json);
    HpBar *hpBar;

private:
    int axis;
    qreal angle;
    QPolygonF polygonShape;
};
#endif // PENTAGON_H
