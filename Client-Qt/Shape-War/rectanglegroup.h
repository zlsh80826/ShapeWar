#ifndef RECTANGLEGROUP_H
#define RECTANGLEGROUP_H
#include <QGraphicsScene>
#include <QJsonObject>
#include <QVector>
#include <rectangle.h>

class RectangleGroup {

public:
    RectangleGroup();
    RectangleGroup(int);
    void addToParent(QGraphicsScene *);
    void read(const QJsonObject &json);

private:
    int count;
    QVector<Rectangle *> rectangles;
};

#endif // RECTANGLEGROUP_H
