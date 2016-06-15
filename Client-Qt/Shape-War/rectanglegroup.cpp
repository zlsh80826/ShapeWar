#include "rectanglegroup.h"
#include <QJsonArray>

RectangleGroup::RectangleGroup() {
    this->count = 300;
    for (int i = 0; i < count; ++i)
        rectangles.append(new Rectangle());
}

void RectangleGroup::addToParent(QGraphicsScene *parent) {
    for (int i = 0; i < count; ++i) {
        parent->addItem(rectangles[i]);
    }
}

void RectangleGroup::read(const QJsonObject &json) {
    QJsonArray rectangleInfo = json["rectangles"].toArray();
    for (int i = 0; (i < rectangleInfo.size()) && i < rectangles.size(); ++i) {
        rectangles[i]->read(rectangleInfo[i].toObject());
    }
}
