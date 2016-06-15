#ifndef TRIANGLEGROUP_H
#define TRIANGLEGROUP_H
#include "triangle.h"
#include <QGraphicsScene>
#include <QVector>

class TriangleGroup {

public:
    TriangleGroup();
    TriangleGroup(int);
    void addToParent(QGraphicsScene *);
    void read(const QJsonObject &json);

private:
    int count;
    QVector<Triangle *> triangles;
};

#endif // TRIANGLEGROUP_H
