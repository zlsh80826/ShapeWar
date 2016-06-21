#include <QKeyEvent>
#include <QPainter>
#include <QtMath>

#include "hero.h"

/*!
 * \brief Hero::Hero constructor
 */
Hero::Hero() {
    this->width = 60;
    // set all flags of the item 0 execpt QGraphicsItem::ItemIsFocusable 1
    this->setFlags(QGraphicsItem::ItemIsFocusable);

    // describe the shape points for QPolygon to know the shape of this hero's berrel
    QVector<QPoint> shapePoint;
    double radian60 = qDegreesToRadians(30.0);
    shapePoint.append(
        QPoint(qCos(radian60) * width / 2 + 3, -qSin(radian60) * width / 2));
    shapePoint.append(
        QPoint(qCos(radian60) * width / 2 + 30, -qSin(radian60) * width / 2));
    shapePoint.append(
        QPoint(qCos(radian60) * width / 2 + 30, qSin(radian60) * width / 2));
    shapePoint.append(
        QPoint(qCos(radian60) * width / 2 + 3, qSin(radian60) * width / 2));
    this->barrel = QPolygon(shapePoint);

    // new nhBar and connect signal of die and reborn to their corresponding slots ( for animation )
    this->hpBar = new HpBar(10000, 60, 40);
    connect(hpBar, SIGNAL(dieSignal()), this, SLOT(onDieSignal()));
    connect(hpBar, SIGNAL(rebornSignal()), this, SLOT(onRebornSignal()));

    // container of this hero's bullets
    bullets = new PolygonGroup<Bullet>(0, 100);
}

/*!
 * \brief Hero::boundingRect mark the rect area to let view know weather
 *  to draw / update this graphics item (the same idea of bullet::boudingRect)
 * \return the rect area
 */
QRectF Hero::boundingRect() const {
    qreal halfPenWidth = 3 / 2;
    // need to more exactly
    return QRectF(-width - halfPenWidth, -width * 2 - halfPenWidth,
                  width * 2 + halfPenWidth, width * 4 + halfPenWidth);
}

/*!
 * \brief Hero::paint (the same idea of bullet::paint)
 * \param painter
 * \param option
 * \param widget
 */
void Hero::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                 QWidget *widget) {
    (void)option;
    (void)widget;
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 255));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(153, 153, 153, 255), Qt::SolidPattern));
    painter->drawPolygon(barrel);
    painter->setBrush(QBrush(QColor(0, 178, 255, 255), Qt::SolidPattern));
    painter->drawEllipse(-width / 2, -width / 2, width, width);
}

/*!
 * \brief Hero::shape (the same idea of bullet::shape)
 * \return
 */
QPainterPath Hero::shape() const {
    QPainterPath path;
    path.addPolygon(barrel);
    path.addEllipse(-width / 2, -width / 2, width, width);
    return path;
}

/*!
 * \brief Hero::read_player read and update the hero's information from server
 * \param data
 */
void Hero::read_player(const QJsonObject &data) {
    this->setRotation(data["angle"].toDouble());
    this->setX(data["x"].toDouble());
    this->setY(data["y"].toDouble());
    this->hpBar->setHp(data["currentHp"].toInt(), data["maxHp"].toInt());
    this->hpBar->setPos(this->x(), this->y());
    this->bullets->read(data["bullets"].toArray());
}

/*!
 * \brief Hero::onDieSignal animates the dying process of this hero
 */
void Hero::onDieSignal() {
    this->hpBar->setVisible(false);
    disappearTimer = new QTimer(this);
    disconnect(disappearTimer, SIGNAL(timeout()), this,
               SLOT(decreaseOpacity()));
    connect(disappearTimer, SIGNAL(timeout()), this, SLOT(decreaseOpacity()));
    disappearTimer->start(20);
}

/*!
 * \brief Hero::onRebornSignal animates the rebirth process of this hero
 */
void Hero::onRebornSignal() {
    this->hpBar->setVisible(true);
    appearTimer = new QTimer(this);
    disconnect(appearTimer, SIGNAL(timeout()), this, SLOT(increaseOpacity()));
    connect(appearTimer, SIGNAL(timeout()), this, SLOT(increaseOpacity()));
    appearTimer->start(20);
}

/*!
 * \brief Hero::decreaseOpacity helps to animate the dying process by setting opacity
 */
void Hero::decreaseOpacity() {
    if (this->opacity() <= 0.03) {
        this->setOpacity(0);
        this->disappearTimer->stop();
        return;
    }
    this->setOpacity(this->opacity() - 0.03);
}

/*!
 * \brief Hero::increaseOpacity helps to animate the rebirth process by setting opacity
 */
void Hero::increaseOpacity() {
    if (this->opacity() >= 0.97) {
        this->setOpacity(1);
        this->appearTimer->stop();
        return;
    }
    this->setOpacity(this->opacity() + 0.03);
}
