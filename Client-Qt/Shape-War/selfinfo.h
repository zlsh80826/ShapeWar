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
    void setExp(int, int);
    void setMaxScore(int value);

private:
    QString name;
    int score;
    int lv;
    int exp;
    int max_exp;
    int targetExp;
    int targetScore;
    int maxScore;
    qreal expWidth;
    qreal scoreWidth;
    qreal maxExpWidth;
    qreal maxScoreWidth;
    QTimer *expTimer;
    QTimer *scoreTimer;

    QString lv_str = "Lv: 1";
    QString scr_str = "Score: 0";

private slots:
    void expAni();
    void scoreAni();
};

#endif // SELFINFO_H
