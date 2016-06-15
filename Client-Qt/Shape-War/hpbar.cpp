#include "hpbar.h"
#include <QPainter>
#include <QDebug>

HpBar::HpBar() {
    this->curHp = 1;
    this->maxHp = 1;
    this->width = 60;
    this->offsetY = 60;
    this->curHpWidth = width * (curHp / maxHp);
    this->revealTimer = new QTimer(this);
    QObject::connect(this->revealTimer, SIGNAL(timeout()), this,
            SLOT(decreaseOpacity()));
}

HpBar::HpBar(qreal initHp, qreal width, qreal offsetY) {
    this->curHp = initHp;
    this->maxHp = initHp;
    this->width = width;
    this->offsetY = offsetY;
    this->curHpWidth = width * (curHp / maxHp);
    this->revealTimer = new QTimer(this);
    connect(this->revealTimer, SIGNAL(timeout()), this,
            SLOT(decreaseOpacity()));
}

void HpBar::setHp(int curHp, int maxHp) {
    if (curHp == maxHp) {
        if(!this->revealTimer->isActive())
            this->revealTimer->start(20);
    }else{
        this->revealTimer->stop();
        this->setOpacity(255);
    }
    this->curHp = curHp;
    this->maxHp = maxHp;
    this->curHpWidth = width * (this->curHp / this->maxHp);
}

QRectF HpBar::boundingRect() const {
    qreal penWidth = 3;
    return QRectF(-width / 2 - penWidth, offsetY, width+penWidth, 5 + penWidth);
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
}

QPainterPath HpBar::shape() const {
    QPainterPath path;
    path.addEllipse(-width / 2, offsetY, curHpWidth, 6);
    path.addEllipse(-width / 2, offsetY, width, 6);
    return path;
}

void HpBar::decreaseOpacity() {
    this->setOpacity(this->opacity() - 0.01);
}
