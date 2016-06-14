#ifndef HERO_H
#define HERO_H
#include <QGraphicsObject>
#include <QPolygon>
#include <QJsonObject>
#include <QJsonArray>
#include <hpbar.h>

class Hero : public QGraphicsObject {
    Q_OBJECT
public:
    Hero();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void setTargetAngle(qreal targetAngle);
    void read(const QJsonObject &json);
    HpBar* hpBar;

private:
    qreal width;
    qreal targetAngle;
    int maxHp;
    int currentHp;
    int experience;
    int level;
    int passives[8];
    QPolygon barrel;
};

#endif // HERO_H
