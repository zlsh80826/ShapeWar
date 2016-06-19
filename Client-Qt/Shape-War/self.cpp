#include "self.h"

Self::Self() : Hero() {
    this->info = new SelfInfo();
    this->upgradePoints = 0;
    for(int i=0 ; i<8 ; i++) {
        passives[i] = 0;
    }
}

void Self::read(const QJsonObject &json) {
    QJsonObject instance = json["self"].toObject();
    this->setX(instance["x"].toDouble());
    this->setY(instance["y"].toDouble());
    this->setUpgradePoints(instance["upgradePoints"].toInt());

    this->hpBar->setPos(this->x(), this->y());
    this->hpBar->setHp(instance["currentHp"].toInt(),
                       instance["maxHp"].toInt());
    this->bullets->read(instance["bullets"].toArray());

    this->info->setLv(instance["level"].toInt());
    this->info->setExp(instance["experience"].toInt(), instance["max_exp"].toInt());


    QJsonArray passivesArray = instance["passives"].toArray();
    for (int i = 0; i < passivesArray.size(); ++i) {
        int value = passivesArray[i].toInt();
        if(passives[i] != value) {
            passives[i] = value;
            emit(passiveChanged(i, value));
        }
    }
}

void Self::setInfoPos(QPointF pos) {
    info->setPos(pos);
}

int Self::getUpgradePoints() const {
    return upgradePoints;
}

int Self::getPassiveLevel(int i)
{
    return passives[i];
}

void Self::setUpgradePoints(int value) {
    if (upgradePoints == value)
        return;
    upgradePoints = value;
    emit upgradePointsChanged();
}
