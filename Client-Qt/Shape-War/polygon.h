#ifndef POLYGON_H
#define POLYGON_H
#include <QColor>
#include <QGraphicsObject>
#include <QJsonObject>
#include <QPolygonF>
#include <QTimer>
#include <hpbar.h>

class Polygon : public QGraphicsObject {
    Q_OBJECT
public:
    Polygon(int);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape() const;
    void read(const QJsonObject &json);
    HpBar *hpBar;

protected:
    int axis;
    QPolygon polygonShape;

private:
    qreal angle;
    int stage;
    int edgeCount;
    enum { ACTIVE, DISAPPEARING, INACTIVE };
    QTimer *disappearTimer;
    QTimer *reviveTimer;
    void setStage(bool);
    void disappear();
    void constructPolygon();

private slots:
    void decreaseOpacity();
    void increaseOpacity();
};

#endif // POLYGON_H
