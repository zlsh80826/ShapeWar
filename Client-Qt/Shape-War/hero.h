#ifndef HERO_H
#define HERO_H
#include <QGraphicsObject>
#include <QPolygon>
#include <QJsonObject>
#include <QJsonArray>

class Hero : public QGraphicsObject {
    Q_OBJECT
public:
    Hero();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void setTargetAngle(qreal targetAngle);
    void drawHp();

protected:
    qreal width;
    int maxHp;
    int currentHp;
    QPolygon barrel;

public:
    void read_player(const QJsonObject&);
};

#endif // HERO_H
