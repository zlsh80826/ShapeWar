#include "hpbar.h"
#include <QDebug>
#include <QPainter>

/*!
 * \brief HpBar::HpBar constructor
 */
HpBar::HpBar() {
    // setup initial values
    this->curHp = 1;
    this->maxHp = 1;
    this->width = 60;
    this->offsetY = 60;
    this->curHpWidth = width * (curHp / maxHp);

    // new timer for hp bar (for animatation of the disappearing/appearing process)
    this->revealTimer = new QTimer(this);
    QObject::connect(this->revealTimer, SIGNAL(timeout()), this,
                     SLOT(decreaseOpacity()));
}
/*!
 * \brief HpBar::HpBar another constructor with setting of some values
 * \param initHp
 * \param width
 * \param offsetY
 */
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

/*!
 * \brief HpBar::setHp setter of the current HP and max HP, emitting die/reborn signals when necessary
 *  and controling the animation process
 * \param curHp
 * \param maxHp
 */
void HpBar::setHp(int curHp, int maxHp) {
    if (this->curHp > 0 && curHp <= 0)
        emit(dieSignal());
    if (this->curHp <= 0 && curHp > 0)
        emit(rebornSignal());

    if (curHp == maxHp) {
        if (!this->revealTimer->isActive())
            this->revealTimer->start(20);
    } else {
        this->revealTimer->stop();
        this->setOpacity(255);
    }
    if (curHp != this->curHp || maxHp != this->maxHp) {
        this->curHp = curHp;
        this->maxHp = maxHp;
        this->curHpWidth = width * (this->curHp / this->maxHp);
        this->update(this->boundingRect());
    }
}
/*!
 * \brief HpBar::boundingRect  (the same idea of bullet::boudingRect)
 * \return
 */
QRectF HpBar::boundingRect() const {
    qreal penWidth = 3;
    return QRectF(-width / 2 - penWidth, offsetY, width + penWidth,
                  5 + penWidth);
}

/*!
 * \brief HpBar::paint (the same idea of bullet::paint)
 * \param painter
 * \param option
 * \param widget
 */
void HpBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                  QWidget *widget) {
    (void)option;
    (void)widget;
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

/*!
 * \brief HpBar::shape (the same idea of bullet::shape)
 * \return
 */
QPainterPath HpBar::shape() const {
    QPainterPath path;
    path.addEllipse(-width / 2, offsetY, curHpWidth, 6);
    path.addEllipse(-width / 2, offsetY, width, 6);
    return path;
}

/*!
 * \brief HpBar::decreaseOpacity helps the animation for hp bar's reveal by setting opacity
 */
void HpBar::decreaseOpacity() {
    this->setOpacity(this->opacity() - 0.01);
}
