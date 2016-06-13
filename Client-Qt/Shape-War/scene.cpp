#include <scene.h>
#include <hero.h>
#include <QPainter>

Scene::Scene(QWidget *parent) : QGraphicsView(parent) {
    this->width = 800;
    this->height = 600;
    this->margin = 10;
    this->setFocus();
    initView();
}

void Scene::initView() {
    setRenderHint(QPainter::Antialiasing, true);
    setCacheMode(CacheBackground);
    setWindowTitle("Shape-War");

    // size control, maybe not necessary
    setMinimumSize(width + margin, height + margin);
    setMaximumSize(width + margin, height + margin);

    QGraphicsScene* scene = new QGraphicsScene;
    scene->setSceneRect(5, 5, width, height);
    setScene(scene);

    self = new Hero();
    self -> setPos(100, 200);
    scene -> addItem(self);

    startGame();
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect) {
    // need to change position to global
    QColor backgroundColor(10, 10, 255, 30);
    painter->setBrush(backgroundColor);
    painter->setPen(backgroundColor);
    for (int i = -5; i <= width + 20; i += 20) {
        painter->drawLine(i, 5, i, height + 5);
    }

    for (int i = -5; i <= height + 20; i+=20) {
        painter->drawLine(5, i, width + 5, i);
    }

}


void Scene::startGame() {
    initGame();
}

void Scene::initGame() {

}

void Scene::gameOver() {

}

void Scene::keyPressEvent(QKeyEvent *event) {
    // movement is just for testing
    // whether to move to judged by server instead of here
    const qreal moveAmount = 3.0;

    switch( event -> key() ){
        case Qt::Key_W :
            this->key_w_pressed = true;
            self->moveBy(0, -moveAmount);
            break;
        case Qt::Key_A :
            this->key_a_pressed = true;
            self->moveBy(-moveAmount, 0);
            break;
        case Qt::Key_S :
            this->key_s_pressed = true;
            self->moveBy(0, moveAmount);
            break;
        case Qt::Key_D :
            this->key_d_pressed = true;
            self->moveBy(moveAmount, 0);
            break;
    }
}
void Scene::mousePressEvent(QMouseEvent *event) {
    mouseClicked = true;
}
void Scene::mouseMoveEvent(QMouseEvent *event) {
    QPointF mouseP = event->pos();
    QPointF selfP = self->pos();
    qreal tangent =  ( mouseP.y() - selfP.y() ) / (mouseP.x() - selfP.x());
    qreal targetAngle = atan( tangent );
    if( mouseP.x() < selfP.x() ) targetAngle += M_PI;
    else if( mouseP.y() < selfP.y() ) targetAngle += M_PI*2;
    self->setTargetAngle( targetAngle );
    printf("degree of mouse relative to self is: %f", targetAngle * 180.0 / 3.14 );
}
