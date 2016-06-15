#include "self.h"

Self::Self() : Hero() {
    this->info = new SelfInfo();
}

void Self::read(const QJsonObject &json) {
    QJsonObject instance = json["self"].toObject();
    this->setX(instance["x"].toDouble());
    this->setY(instance["y"].toDouble());
    this->experience = instance["experience"].toInt();
    this->level = instance["level"].toInt();
    this->hpBar->setPos(this->x(), this->y());
    this->hpBar->setHp(instance["maxHp"].toInt(), instance["currentHp"].toInt());

    this->info->lv = this->level;
    this->info->exp =this->experience;

    QJsonArray passivesArray = instance["passives"].toArray();
    for (int i = 0; i < passivesArray.size(); ++i) {
        this->passives[i] = passivesArray[i].toInt();
    }
}
