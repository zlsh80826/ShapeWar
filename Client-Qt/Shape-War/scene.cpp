#include <scene.h>
#include <hero.h>
#include <QPainter>

Scene::Scene(QWidget *parent) : QGraphicsScene(parent) {
    this->width = 2000;
    this->height = 1500;
    this->margin = 10;
    initGame();
    startGame();
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect) {
    // need to change position to global
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

}

void Scene::initGame() {

    this->setSceneRect(5, 5, width, height);
    this->backgroundColor = QColor(10, 10, 255, 30);
    // self should not be constructed here after server is completed ?
    self = new Hero();
    self -> setPos(100, 200);
    this -> addItem(self);

    // test monster
    testTriangle = new Triangle();
    testTriangle -> setPos(100, 300);
    this -> addItem(testTriangle);

    testRectangle = new Rectangle();
    testRectangle -> setPos(200, 200);
    this -> addItem(testRectangle);

    testPentagon = new Pentagon();
    testPentagon -> setPos(200, 300);
    this -> addItem(testPentagon);

    testBullet = new Bullet();
    testBullet -> setPos(300, 250);
    this -> addItem(testBullet);
}

void Scene::gameOver() {

}

