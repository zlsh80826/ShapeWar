#ifndef PENTAGONGROUP_H
#define PENTAGONGROUP_H
#include <QGraphicsScene>
#include <QJsonObject>
#include <QVector>
#include <pentagon.h>

class PentagonGroup {
public:
    PentagonGroup();
    PentagonGroup(int);
    void addToParent(QGraphicsScene *);
    void read(const QJsonObject &json);

private:
    int count;
    QVector<Pentagon *> pentagons;
};

#endif // PENTAGONGROUP_H
