#ifndef HERO_H
#define HERO_H
#include <QGraphicsObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QPolygon>
#include <hpbar.h>
#include <bullet.h>
#include <polygongroup.h>
#include <QTimer>

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
private slots:
    void onDieSignal();
    void decreaseOpacity();
private:
    QTimer *disappearTimer;
};

#endif // HERO_H
