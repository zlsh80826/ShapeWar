#include "self.h"

Self::Self() : Hero() {
    this->info = new SelfInfo();
    this->upgradePoints = 0;
}

void Self::read(const QJsonObject &json) {
    QJsonObject instance = json["self"].toObject();
    this->setX(instance["x"].toDouble());
    this->setY(instance["y"].toDouble());
    this->experience = instance["experience"].toInt();
    this->level = instance["level"].toInt();
    this->setUpgradePoints(instance["upgradePoints"].toInt());

    this->hpBar->setPos(this->x(), this->y());
    this->hpBar->setHp(instance["currentHp"].toInt(),
                       instance["maxHp"].toInt());
    this->bullets->read(instance["bullets"].toArray());

    this->info->setLv(this->level);
    this->info->setExp(instance["experience"].toInt());

    QJsonArray passivesArray = instance["passives"].toArray();
    for (int i = 0; i < passivesArray.size(); ++i) {
        this->passives[i] = passivesArray[i].toInt();
    }
}

void Self::setInfoPos(QPointF pos) {
    info->setPos(pos);
}

int Self::getUpgradePoints() const {
    return upgradePoints;
}

void Self::setUpgradePoints(int value) {
    if (upgradePoints == value)
        return;
    if (upgradePoints < value) {
        upgradePoints = value;
        emit upgradePointsChanged();
    } else {
        --upgradePoints;
    }
}
