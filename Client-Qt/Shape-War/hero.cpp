#include <hero.h>
#include <QPainter>
#include <QKeyEvent>
#include <QtMath>

Hero::Hero() {
    this -> width = 60;
    this -> setFlags(QGraphicsItem::ItemIsFocusable);
    QVector<QPoint> shapePoint;
    double radian60 = qDegreesToRadians(30.0);
    shapePoint.append(QPoint(qCos(radian60)*width/2 + 3, -qSin(radian60)*width/2 ));
    shapePoint.append(QPoint(qCos(radian60)*width/2 + 30 , -qSin(radian60)*width/2 ));
    shapePoint.append(QPoint(qCos(radian60)*width/2 + 30, qSin(radian60)*width/2 ));
    shapePoint.append(QPoint(qCos(radian60)*width/2 + 3, qSin(radian60)*width/2 ));
    this -> barrel = QPolygon(shapePoint);

    // test hpBar
    this -> hpBar = new HpBar(10000, 60, 60);
    connect(this, SIGNAL(xChanged()), this->hpBar, SLOT(setPos(this->x(), this->y())));
    connect(this, SIGNAL(yChanged()), this->hpBar, SLOT(setPos(this->x(), this->y())));
}

QRectF Hero::boundingRect() const {
    qreal halfPenWidth = 3/2;
    // need to more exactly
    return QRectF( -width - halfPenWidth, -width*2 - halfPenWidth, width*2 + halfPenWidth, width*4 + halfPenWidth);
}

void Hero::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 255));
    painter -> setRenderHint( QPainter::Antialiasing );
    painter -> setPen(pen);
    painter -> setBrush(QBrush(QColor(153, 153, 153, 255), Qt::SolidPattern));
    painter -> drawPolygon(barrel);
    painter -> setBrush(QBrush(QColor(0, 178, 255, 255), Qt::SolidPattern));
    painter -> drawEllipse(-width/2, -width/2, width, width);
    this -> drawHp(painter);
}

QPainterPath Hero::shape() const {
    QPainterPath path;
    path.addPolygon(barrel);
    path.addEllipse(-width/2, -width/2, width, width);
    return path;
}

void Hero::setTargetAngle(qreal targetAngle) {
    this->targetAngle = targetAngle;
}

void Hero::read(const QJsonObject &json){
    QJsonObject instance = json["self"].toObject();
    this -> setX(instance["x"].toDouble());
    this -> setY(instance["y"].toDouble());
    this -> setTargetAngle(instance["angle"].toDouble());
    this -> maxHp = instance["maxHp"].toInt();
    this -> currentHp = instance["currentHp"].toInt();
    this -> experience = instance["experience"].toInt();
    this -> level = instance["level"].toInt();

    QJsonArray passivesArray = instance["passives"].toArray();
    for(int i=0; i < passivesArray.size(); ++i){
        this -> passives[i] = passivesArray[i].toInt();
    }
}

void Hero::drawHp(QPainter* painter){
    // no idea
}
