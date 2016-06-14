#ifndef HERO_H
#define HERO_H
#include <QGraphicsObject>
#include <QJsonObject>

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

private:
    qreal width;
    qreal targetAngle;
};

#endif // HERO_H
