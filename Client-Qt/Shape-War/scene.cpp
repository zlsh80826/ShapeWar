#include <scene.h>
#include <hero.h>
#include <QPainter>

Scene::Scene(QWidget *parent) : QGraphicsScene(parent) {
    this->width = 2000;
    this->height = 1500;
    this->margin = 10;
    initView();
}

void Scene::initView() {

    this->setSceneRect(5, 5, width, height);

    self = new Hero();
    self -> setPos(100, 200);
    this -> addItem(self);

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
  /*
    recvDelayTimer = new QTimer(this);
    connect(sendDelayTimer, SIGNAL(timeout()), this, SLOT(sendToServer()) );
    connect(recvDelayTimer, SIGNAL(timeout()), this, SLOT(recvFromServer()) );*/

}

void Scene::gameOver() {

}

