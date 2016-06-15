#ifndef SELF_H
#define SELF_H

#include "hero.h"
#include "selfinfo.h"
class Self : public Hero {
public:
    Self();
    void read(const QJsonObject &json);

private:
    int experience;
    int level;
    int passives[8];
    SelfInfo* info;
};

#endif // SELF_H
