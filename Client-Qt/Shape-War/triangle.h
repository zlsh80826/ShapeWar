#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <QGraphicsObject>
#include <QJsonObject>
#include <QPolygonF>
#include <hpbar.h>

class Triangle : public QGraphicsObject {
    Q_OBJECT
public:
    Triangle();
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
    int hp;
    int maxHp;
    void update();
};

#endif // TRIANGLE_H
