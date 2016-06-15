#ifndef HERO_H
#define HERO_H
#include <QGraphicsObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QPolygon>
#include <hpbar.h>

class Hero : public QGraphicsObject {
    Q_OBJECT
public:
    Hero();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape() const;
    void setTargetAngle(qreal targetAngle);
    HpBar *hpBar;

protected:
    qreal width;
    int maxHp;
    int currentHp;
    QPolygon barrel;

public:
    void read_player(const QJsonObject &);
    void update();
};

#endif // HERO_H
