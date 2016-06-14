#ifndef BULLET_H
#define BULLET_H
#include <QGraphicsObject>
#include <QJsonObject>

class Bullet : public QGraphicsObject {
    Q_OBJECT
public:
    Bullet();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void read(const QJsonObject &json);

private:
    int radius;
};
#endif // BULLET_H
