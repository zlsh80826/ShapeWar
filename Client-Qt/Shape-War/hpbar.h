#ifndef HPBAR_H
#define HPBAR_H
#include <QGraphicsObject>

class HpBar : public QGraphicsObject {
    Q_OBJECT
public:
    HpBar();
    HpBar(qreal initHp, qreal width, qreal offsetY);
    void setHp(int curHp, int width);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

private:
    qreal curHp;
    qreal maxHp;
    qreal width;
    qreal offsetY;
    qreal curHpWidth;
};

#endif // HPBAR_H
