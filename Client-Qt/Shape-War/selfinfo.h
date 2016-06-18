#ifndef SELFINFO_H
#define SELFINFO_H
#include <QGraphicsObject>

class SelfInfo : public QGraphicsObject {
    Q_OBJECT
public:
    SelfInfo();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setName(QString);
    void setLv(int);
    void setScore(int);
    void setExp(int);

private:
    QString name;
    int score;
    int lv;
    int exp;
    qreal targetExp;
    int maxScore;
    qreal expWidth;
    qreal scoreWidth;
    qreal maxExpWidth;
    qreal maxScoreWidth;
    QTimer* timer;

private slots:
    void expAni();
};

#endif // SELFINFO_H
