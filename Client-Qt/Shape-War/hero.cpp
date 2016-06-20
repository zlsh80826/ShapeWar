#include <QKeyEvent>
#include <QPainter>
#include <QtMath>

#include "hero.h"

Hero::Hero() {
    this->width = 60;
    this->setFlags(QGraphicsItem::ItemIsFocusable);
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

    // test hpBar
    this->hpBar = new HpBar(10000, 60, 40);
    connect(hpBar, SIGNAL(dieSignal()), this, SLOT(onDieSignal()));
    connect(hpBar, SIGNAL(rebornSignal()), this, SLOT(onRebornSignal()));
    bullets = new PolygonGroup<Bullet>(0, 200);
}

QRectF Hero::boundingRect() const {
    qreal halfPenWidth = 3 / 2;
    // need to more exactly
    return QRectF(-width - halfPenWidth, -width * 2 - halfPenWidth,
                  width * 2 + halfPenWidth, width * 4 + halfPenWidth);
}

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

QPainterPath Hero::shape() const {
    QPainterPath path;
    path.addPolygon(barrel);
    path.addEllipse(-width / 2, -width / 2, width, width);
    return path;
}

void Hero::read_player(const QJsonObject &data) {
    this->setRotation(data["angle"].toDouble());
    this->setX(data["x"].toDouble());
    this->setY(data["y"].toDouble());
    this->hpBar->setHp(data["currentHp"].toInt(), data["maxHp"].toInt());
    this->hpBar->setPos(this->x(), this->y());
    this->bullets->read(data["bullets"].toArray());
}

void Hero::onDieSignal() {
    printf("die");
    this->hpBar->setVisible(false);
    disappearTimer = new QTimer(this);
    connect(disappearTimer, SIGNAL(timeout()), this, SLOT(decreaseOpacity()));
    disappearTimer->start(20);
}

void Hero::onRebornSignal() {
    printf("reborn");
    this->hpBar->setVisible(true);
    appearTimer = new QTimer(this);
    connect(appearTimer, SIGNAL(timeout()), this, SLOT(increaseOpacity()));
    appearTimer->start(20);
}

void Hero::decreaseOpacity() {
    if (this->opacity() <= 0.03) {
        this->setOpacity(0);
        this->disappearTimer->stop();
        return;
    }
    this->setOpacity(this->opacity() - 0.03);
}

void Hero::increaseOpacity() {
    if (this->opacity() >= 0.97) {
        this->setOpacity(1);
        this->disappearTimer->stop();
        return;
    }
    this->setOpacity(this->opacity() + 0.03);
}
