#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsObject>
#include <QJsonObject>
#include <QTimer>

class Bullet : public QGraphicsObject {
    Q_OBJECT
public:
    Bullet(int);
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape() const;
    void read(const QJsonObject &json);

private:
    int radius;
    void disappear();
    QTimer *disappearTimer;
    void setStage(bool);
    int stage;
    enum { ACTIVE, DISAPPEARING, INACTIVE };

private slots:
    void decreaseOpacity();
};
#endif // BULLET_H
