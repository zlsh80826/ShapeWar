#ifndef HERO_H
#define HERO_H
#include <QGraphicsObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QPolygon>
#include <QTimer>
#include <bullet.h>
#include <hpbar.h>
#include <polygongroup.h>

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
    PolygonGroup<Bullet> *bullets;

protected:
    qreal width;
    QPolygon barrel;

public:
    void read_player(const QJsonObject &);

public slots:
    void onDieSignal();
    void onRebornSignal();
    void decreaseOpacity();
    void increaseOpacity();

private:
    QTimer *disappearTimer;
    QTimer *appearTimer;
};

#endif // HERO_H
