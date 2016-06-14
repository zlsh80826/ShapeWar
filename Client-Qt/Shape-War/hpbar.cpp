#include "hpbar.h"
#include <QPainter>

HpBar::HpBar() {
    this->curHp = 1;
    this->maxHp = 1;
    this->width = 60;
    this->offsetY = 60;
    this->curHpWidth = width * (curHp / maxHp);
}

HpBar::HpBar(qreal initHp, qreal width, qreal offsetY) {
    this->curHp = initHp;
    this->maxHp = initHp;
    this->width = width;
    this->offsetY = offsetY;
    this->curHpWidth = width * (curHp / maxHp);
}

void HpBar::setHp(int curHp, int maxHp) {
    this->curHp = curHp;
    this->maxHp = maxHp;
    this->curHpWidth = width * (this->curHp / this->maxHp);
}

QRectF HpBar::boundingRect() const {
    qreal penWidth = 3;
    return QRectF(-width / 2 - penWidth, offsetY, width, 5 + penWidth);
}

void HpBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 255));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(85, 85, 85, 255), Qt::SolidPattern));
    painter->drawRect(-width / 2, offsetY, width, 6);
    painter->setBrush(QBrush(QColor(134, 198, 128, 255), Qt::SolidPattern));
    painter->drawRect(-width / 2, offsetY, curHpWidth, 6);
    // painter -> drawEllipse(-width/2, offsetY, curHpWidth, 6);
    // painter -> drawEllipse(-width/2, offsetY, width, 6);
}

QPainterPath HpBar::shape() const {
    QPainterPath path;
    path.addEllipse(-width / 2, offsetY, curHpWidth, 6);
    path.addEllipse(-width / 2, offsetY, width, 6);
    return path;
}
