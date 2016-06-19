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
    int getPassiveLevel(int i);
    const int passiveMax = 8;
signals:
    void upgradePointsChanged();
    void passiveChanged(int id, int changeToValue);

private:
    int passives[8];
    int upgradePoints;
};

#endif // SELF_H
