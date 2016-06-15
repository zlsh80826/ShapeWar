#ifndef SELFINFO_H
#define SELFINFO_H
#include <QGraphicsObject>

class SelfInfo : QGraphicsObject{

public:
    SelfInfo();
    SelfInfo(QString);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

private:
    QString name;
    int score;
    int lv;
    int exp;
    int maxScore;
};

#endif // SELFINFO_H
