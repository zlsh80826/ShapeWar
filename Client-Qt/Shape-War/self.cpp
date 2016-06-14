#include "self.h"

Self::Self(): Hero()
{

}


void Self::read(const QJsonObject &json){
    QJsonObject instance = json["self"].toObject();
    this -> setX(instance["x"].toDouble());
    this -> setY(instance["y"].toDouble());
    this -> maxHp = instance["maxHp"].toInt();
    this -> currentHp = instance["currentHp"].toInt();
    this -> experience = instance["experience"].toInt();
    this -> level = instance["level"].toInt();
    this -> hpBar -> setPos(this->x(), this->y());
    this -> hpBar ->setHp(this->currentHp, this->maxHp);

    QJsonArray passivesArray = instance["passives"].toArray();
    for(int i=0; i < passivesArray.size(); ++i){
        this -> passives[i] = passivesArray[i].toInt();
    }
}
