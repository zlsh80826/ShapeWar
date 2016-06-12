#include <hero.h>
#include <QPainter>
#include <QKeyEvent>

Hero::Hero() {
    this -> width = 60;
    this -> speed = 0;
    this -> maxSpeed = 80;
    this -> setFlags(QGraphicsItem::ItemIsFocusable);
    printf("Hero_constructed");
}

QRectF Hero::boundingRect() const {
    qreal halfPenWidth = 1/2;
    return QRectF( -width/2 - halfPenWidth, -width/2 - halfPenWidth, width + halfPenWidth, width + halfPenWidth);
}

void Hero::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter -> drawRect(-width/2, -width/2, width, width);
}

QPainterPath Hero::shape() const {
    QPainterPath path;
    path.addRect(-width/2, -width/2, width, width);
    return path;
}

void Hero::keyPressEvent(QKeyEvent *event) {
    printf("Key_pressed\n");
    this -> speed += 0.019;
    switch( event -> key() ){
        case Qt::Key_Down :
            moveBy(0, speed);
            break;
        case Qt::Key_Left :
            moveBy(-speed, 0);
            break;
        case Qt::Key_Up :
            moveBy(0, -speed);
            break;
        case Qt::Key_Right :
            moveBy(speed, 0);
            break;
    }
}

void Hero::keyReleaseEvent(QKeyEvent* event) {
    if(event->isAutoRepeat()) {
        event->ignore();
    }
    else {
        this -> speed = 0;
    }
}
