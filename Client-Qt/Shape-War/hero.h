#ifndef HERO_H
#define HERO_H
#include <QGraphicsObject>

class Hero : public QGraphicsObject{
    Q_OBJECT
public:
    Hero();
    // need to optimize to boundingRegion
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

protected:
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    int width;
    qreal speed;
    qreal maxSpeed;
};

#endif // HERO_H
