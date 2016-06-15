#include "selfinfo.h"
#include <QPainter>

SelfInfo::SelfInfo() {
    this -> score = 0;
    this -> maxScore = 50000;
    this -> lv = 1;
    this -> exp = 0;
    this -> setPos(480, 600);
}

SelfInfo::SelfInfo(QString name) {
    this -> score = 0;
    this -> maxScore = 50000;
    this -> lv = 1;
    this -> exp = 0;
    this -> setPos(480, 600);
    this->name = name;
}

void SelfInfo::setName(QString name){
    this->name=name;
}

QRectF SelfInfo::boundingRect(){
    return QRectF(-80, 0, 160, 160);
}

void SelfInfo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 240));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(240, 240, 240, ), Qt::SolidPattern));
    painter->drawText(-50, 0, name);
}
