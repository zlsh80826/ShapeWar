#ifndef SELF_H
#define SELF_H

#include "hero.h"
#include "selfinfo.h"
class Self : public Hero {
    Q_OBJECT
public:
    Self();
    void read(const QJsonObject &json);
    SelfInfo *info;
    const QStringList passiveNames = (QStringList() << "Health Regen"
                                                    << "Max Health"
                                                    << "Body Damage"
                                                    << "Bullet Speed"
                                                    << "Bullet Penetration"
                                                    << "Bullet Damage"
                                                    << "Reload"
                                                    << "Movement Speed");
    void setInfoPos(QPointF);
    int getUpgradePoints() const;
    void setUpgradePoints(int value);
signals:
    void upgradePointsChanged();

private:
    int experience;
    int level;
    int passives[8];
    int upgradePoints;
};

#endif // SELF_H
