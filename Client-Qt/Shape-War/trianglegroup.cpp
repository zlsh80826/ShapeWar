#include "trianglegroup.h"
#include <QJsonArray>

TriangleGroup::TriangleGroup() {
    this->count = 300;
    for (int i = 0; i < count; ++i)
        triangles.append(new Triangle());
}

void TriangleGroup::addToParent(QGraphicsScene *parent) {
    for (int i = 0; i < count; ++i) {
        parent->addItem(triangles[i]);
        parent->addItem(triangles[i]->hpBar);
    }
}

void TriangleGroup::read(const QJsonObject &json) {
    QJsonArray triangleInfo = json["triangles"].toArray();
    for (int i = 0; (i < triangleInfo.size()) && i < triangles.size(); ++i) {
        triangles[i]->read(triangleInfo[i].toObject());
    }
}
