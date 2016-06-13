#include <triangle.h>
#include <QPainter>

Triangle::Triangle(){
    this -> edge = 20;
    QVector<QPoint> shapePoint;
    shapePoint.append(QPoint(0, -(edge*2)/3));
    shapePoint.append(QPoint(-edge/2, edge/3));
    shapePoint.append(QPoint(edge/2, edge/3));
    shapePoint.append(QPoint(0, -(edge*2)/3));
    this -> polygonShape = QPolygon(shapePoint);
}

QRectF Triangle::boundingRect() const{
    qreal halfPenWidth = 1;
    return QRectF( -edge/2 - halfPenWidth, (-edge*2)/3 - halfPenWidth, edge + halfPenWidth, edge + halfPenWidth);
}

void Triangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter -> drawPath(this->shape());
}

QPainterPath Triangle::shape() const{
    QPainterPath path;
    path.addPolygon(polygonShape);
    return path;
}

void Triangle::setTargetAngle(qreal targetAngle){

}
