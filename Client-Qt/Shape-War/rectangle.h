#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <QGraphicsObject>
#include <QJsonObject>
#include <QPolygonF>
#include <QTimer>
#include <hpbar.h>

class Rectangle : public QGraphicsObject {
    Q_OBJECT
public:
    Rectangle();
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
    QPolygon polygonShape;
    int stage;
    enum{ACTIVE, DISAPPEARING, INACTIVE};
    QTimer* disappearTimer;
    QTimer* reviveTimer;
    void setStage(bool);
    void disappear();
    void constructPolygon();

private slots:
    void decreaseOpacity();
    void increaseOpacity();
};
#endif // RECTANGLE_H
