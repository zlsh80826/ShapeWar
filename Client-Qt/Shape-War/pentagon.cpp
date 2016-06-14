#include <pentagon.h>
#include <QPainter>
#include <QtMath>

Pentagon::Pentagon(){
    this -> axis = 20;
    QVector<QPoint> shapePoint;
    double radian18 = qDegreesToRadians(18.0);
    double radian54 = qDegreesToRadians(54.0);
    shapePoint.append(QPoint(0, -axis));
    shapePoint.append(QPoint(-qCos(radian18)*axis, -qSin(radian18)*axis));
    shapePoint.append(QPoint(-qCos(radian54)*axis, qSin(radian54)*axis));
    shapePoint.append(QPoint(qCos(radian54)*axis, qSin(radian54)*axis));
    shapePoint.append(QPoint(qCos(radian18)*axis, -qSin(radian18)*axis));
    shapePoint.append(QPoint(0, -axis));
    this -> polygonShape = QPolygon(shapePoint);
}

QRectF Pentagon::boundingRect() const{
    qreal halfPenWidth = 1/2;
    return QRectF( -axis - halfPenWidth, -axis - halfPenWidth, axis*2 + halfPenWidth, axis*2 + halfPenWidth);
}

void Pentagon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter -> setBrush(QBrush(QColor(118, 141, 252, 255), Qt::SolidPattern));
    painter -> setRenderHint( QPainter::Antialiasing );
    painter -> drawPath(this->shape());
}

QPainterPath Pentagon::shape() const{
    QPainterPath path;
    path.addPolygon(polygonShape);
    return path;
}

void Pentagon::read(const QJsonObject &json){
    this -> setX(json["x"].toDouble());
    this -> setY(json["y"].toDouble());
    this -> angle = json["angle"].toDouble();
    this -> hp = json["hp"].toInt();
    this -> maxHp = json["maxHp"].toInt();
}
