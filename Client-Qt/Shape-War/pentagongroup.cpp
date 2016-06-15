#include "pentagongroup.h"
#include "pentagon.h"
#include <QJsonArray>

PentagonGroup::PentagonGroup() {
    this->count = 300;
    for (int i = 0; i < count; ++i)
        pentagons.append(new Pentagon());
}

void PentagonGroup::addToParent(QGraphicsScene *parent) {
    for (int i = 0; i < count; ++i) {
        parent->addItem(pentagons[i]);
    }
}

void PentagonGroup::read(const QJsonObject &json) {
    QJsonArray PentagonInfo = json["pentagons"].toArray();
    for (int i = 0; (i < PentagonInfo.size()) && i < pentagons.size(); ++i) {
        pentagons[i]->read(PentagonInfo[i].toObject());
    }
}
