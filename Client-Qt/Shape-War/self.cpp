#include "self.h"

Self::Self() : Hero() {
    this->info = new SelfInfo();
    this->upgradePoints = 0;
    for (int i = 0; i < 8; i++) {
        passives[i] = 0;
    }
}

void Self::read_info(const QJsonObject &json) {
    QJsonObject instance = json["self"].toObject();
    this->setUpgradePoints(instance["upgradePoints"].toInt());
    this->info->setLv(instance["level"].toInt());
    this->info->setExp(instance["experience"].toInt(),
                       instance["max_exp"].toInt());
    this->info->setScore(instance["score"].toInt());
    this->info->setMaxScore(instance["maxScore"].toInt());

    QJsonArray passivesArray = instance["passives"].toArray();
    for (int i = 0; i < passivesArray.size(); ++i) {
        int value = passivesArray[i].toInt();
        if (passives[i] != value) {
            passives[i] = value;
            emit(passiveChanged(i, value));
        }
    }
}

void Self::read_global(const QJsonObject &data) {
    this->setX(data["x"].toDouble());
    this->setY(data["y"].toDouble());
    this->hpBar->setPos(this->x(), this->y());
    this->bullets->read(data["bullets"].toArray());
    this->hpBar->setHp(data["currentHp"].toInt(), data["maxHp"].toInt());
}

void Self::setInfoPos(QPointF pos) {
    info->setPos(pos);
}

int Self::getUpgradePoints() const {
    return upgradePoints;
}

int Self::getPassiveLevel(int i) {
    return passives[i];
}

void Self::setUpgradePoints(int value) {
    if (upgradePoints == value)
        return;
    upgradePoints = value;
    emit upgradePointsChanged();
}
