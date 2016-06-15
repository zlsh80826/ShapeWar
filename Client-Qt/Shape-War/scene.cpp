#include "scene.h"
#include "hero.h"
#include "logindialog.h"
#include "self.h"
#include <QJsonDocument>
#include <QPainter>
#include <QtWebSockets/QWebSocket>

Scene::Scene(QWidget *parent, const QUrl &url)
    : QGraphicsScene(parent), m_url(url) {
    LoginDialog *loginDialog = new LoginDialog();

    QObject::connect(loginDialog, SIGNAL(acceptLogin(QString &, QString &)),
                     this, SLOT(slotAcceptUserLogin(QString &, QString &)));
    loginDialog->exec();

    this->width = 2000;
    this->height = 1500;
    this->margin = 10;
    initGame();
    startGame();

    connect(&m_webSocket, &QWebSocket::connected, this, &Scene::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Scene::closed);
    m_webSocket.open(QUrl(url));
}

void Scene::drawBackground(QPainter *painter, const QRectF &rect) {
    // need to change position to global
    painter->setBrush(backgroundColor);
    painter->setPen(backgroundColor);
    for (int i = -5; i <= width + 20; i += 20) {
        painter->drawLine(i, 5, i, height + 5);
    }

    for (int i = -5; i <= height + 20; i += 20) {
        painter->drawLine(5, i, width + 5, i);
    }
}

void Scene::startGame() {
}

void Scene::initGame() {

    this->setSceneRect(5, 5, width, height);
    this->backgroundColor = QColor(10, 10, 255, 30);

    // self should not be constructed here after server is completed ?
    self = new Self();
    self->setPos(100, 200);
    this->addItem(self);
    this->addItem(self->hpBar);
    this->self->setZValue(1);

    // test monster
    testTriangle = new Triangle();
    testTriangle->setPos(100, 300);
    this->addItem(testTriangle);

    testRectangle = new Rectangle();
    testRectangle->setPos(200, 200);
    this->addItem(testRectangle);

    testPentagon = new Pentagon();
    testPentagon->setPos(200, 300);
    this->addItem(testPentagon);

    testBullet = new Bullet();
    testBullet->setPos(300, 250);
    this->addItem(testBullet);
}

void Scene::gameOver() {
}

void Scene::onConnected() {
    qDebug() << "WebSocket connected";
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this,
            &Scene::onTextMessageReceived);
    // m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}

void Scene::onTextMessageReceived(QString message) {
    // qDebug().noquote() << "Message received:" << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    const auto &object = doc.object();
    this->self->read(object);
    this->testPentagon->read(object["pentagons"].toArray()[0].toObject());

    int self_id = object["self"].toObject()["id"].toInt();

    for (Hero *hero : heroes) {
        this->removeItem(hero);
        delete hero;
    }
    heroes.clear();

    for (const auto &hero_data : object["players"].toArray()) {
        const auto &hero_object = hero_data.toObject();
        if (hero_object["id"].toInt() != self_id) {
            auto hero = new Hero;
            hero->read_player(hero_object);
            this->addItem(hero);
            heroes.push_back(hero);
        }
    }
}

void Scene::slotAcceptUserLogin(QString &username, QString &password) {
    // test
    qDebug() << "Get username: " << username << ", password: " << password;

    // TODO: send the username and password to server
}
