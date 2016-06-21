#include "bullet.h"
#include <QDebug>
#include <QPainter>

// constructor
Bullet::Bullet(int) {
    this->setOpacity(0);
    this->radius = 20;
    this->disappearTimer = new QTimer(this);
    this->stage = INACTIVE;
    QObject::connect(this->disappearTimer, SIGNAL(timeout()), this,
                     SLOT(decreaseOpacity()));
}
/*!
 * \brief Bullet::boundingRect mark the rect area to let view know weather
 *  to draw / update this graphics item
 * \return the rect area
 */
QRectF Bullet::boundingRect() const {
    qreal halfPenWidth = 2;
    return QRectF(-radius - halfPenWidth, -radius - halfPenWidth,
                  radius * 2 + halfPenWidth, radius * 2 + halfPenWidth);
}

/*!
 * \brief Bullet::paint
 * \param painter performs low-level painting on widgets and other paint devicess
 * \param option describe the parameters needed to draw
 * \param widget If provided, it points to the widget that is being painted on;
 * otherwise, it is 0. For cached painting, widget is always 0
 */
void Bullet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) {
    (void)option;
    (void)widget;
    QPen pen;
    pen.setWidth(4);
    pen.setColor(QColor(85, 85, 85, 255));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(241, 78, 84, 255), Qt::SolidPattern));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawPath(this->shape());
}

/*!
 * \brief Bullet::shape describe the paint path for painter of Bullet::paint
 * \return the path
 */
QPainterPath Bullet::shape() const {
    QPainterPath path;
    path.addEllipse(0, 0, radius, radius);
    return path;
}

/*!
 * \brief Bullet::read read the Json object to update the information of this Bullet
 * \param json the Json object to read
 */
void Bullet::read(const QJsonObject &json) {
    bool next_active = json["visible"].toBool();
    if (((this->stage != ACTIVE) && (next_active == false)) ||
        (this->stage == DISAPPEARING))
        return;
    this->setStage(next_active);
    this->setX(json["x"].toDouble());
    this->setY(json["y"].toDouble());
    this->radius = json["radius"].toInt();
}

/*!
 * \brief Bullet::setStage make this bullet stage from ( ACTIVE -> DISAPPEARING )
 *  or ( DISAPPEARING -> INACTIVE ) or ( INACTIVE -> ACTIVE )
 * \param control
 */
void Bullet::setStage(bool control) {
    if (control == false) {
        this->stage = DISAPPEARING;
        this->disappear();
    } else {
        this->stage = ACTIVE;
        this->setOpacity(1);
    }
}

/*!
 * \brief Bullet::disappear make this bullet gradually disappear
 */
void Bullet::disappear() {
    this->disappearTimer->start(15);
}

/*!
 * \brief Bullet::decreaseOpacity implement the disappear animation using setting of opacity
 */
void Bullet::decreaseOpacity() {
    if (this->opacity() <= 0.05) {
        this->setOpacity(0);
        this->disappearTimer->stop();
        this->stage = INACTIVE;
    }
    this->setOpacity(this->opacity() - 0.08);
    this->radius += 1;
}
