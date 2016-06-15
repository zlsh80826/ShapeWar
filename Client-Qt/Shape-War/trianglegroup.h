#ifndef TRIANGLEGROUP_H
#define TRIANGLEGROUP_H
#include <QVector>
#include <QGraphicsScene>
#include "triangle.h"

class TriangleGroup {

public:
    TriangleGroup();
    TriangleGroup(int);
    void addToParent(QGraphicsScene*);
    void read(const QJsonObject &json);

private:
    int count;
    QVector<Triangle*> triangles;
};

#endif // TRIANGLEGROUP_H
