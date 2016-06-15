#include "scene.h"
#include "hero.h"
#include "logindialog.h"
#include "polygongroup.h"
#include "self.h"
#include "selfinfo.h"
#include <QJsonDocument>
#include <QPainter>
#include <QtWebSockets/QWebSocket>

Scene::Scene(QWidget *parent) : QGraphicsScene(parent) {
    LoginDialog *loginDialog = new LoginDialog();

    QObject::connect(
        loginDialog,
        SIGNAL(acceptLogin(QString &, QString &, QString &, QString &, bool)),
        this, SLOT(slotAcceptUserLogin(QString &, QString &, QString &,
                                       QString &, bool)));
    loginDialog->exec();

    this->width = 5000;
    this->height = 4000;
    this->margin = 10;
    initGame();
    startGame();

    connect(&m_webSocket, &QWebSocket::connected, this, &Scene::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Scene::closed);
    m_webSocket.open(QUrl(*dummy_url));

    sec = new QTimer(0);
    this->recvs = 0;
    connect(sec, SIGNAL(timeout()), this, SLOT(print_freq()));
    sec->start(1000);
}
void Scene::print_freq() {
    qDebug() << "recv per sec: " << this->recvs;
    this->recvs = 0;
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
    // test name
    this->self->info->setName("test");
    this->addItem(this->self->info);
    // test monster
    triangles = new PolygonGroup<Triangle>;
    triangles->addToParent(this);

    rectangles = new PolygonGroup<Rectangle>;
    rectangles->addToParent(this);

    pentagons = new PolygonGroup<Pentagon>;
    pentagons->addToParent(this);

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
    this->recvs++;
    // qDebug().noquote() << "Message received:" << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    const auto &object = doc.object();
    this->self->read(object);
    this->triangles->read(object["triangles"].toArray());
    this->rectangles->read(object["squares"].toArray());
    this->pentagons->read(object["pentagons"].toArray());

    auto self_id = object["self"].toObject()["id"];

    for (Hero *hero : heroes) {
        this->removeItem(hero);
        delete hero;
    }
    heroes.clear();

    for (const auto &hero_data : object["players"].toArray()) {
        const auto &hero_object = hero_data.toObject();
        if (hero_object["id"] != self_id) {
            auto hero = new Hero;
            hero->read_player(hero_object);
            this->addItem(hero);
            heroes.push_back(hero);
        }
    }
}

void Scene::slotAcceptUserLogin(QString &serverIP, QString &port,
                                QString &username, QString &password,
                                bool isAnonymous) {
    // test
    fillServerInfo(serverIP, port);
    qDebug() << "Get serverIP: " << serverIP << ", port: " << port
             << ", username: " << username << ", password: " << password
             << ", anonymous: " << isAnonymous;

    // TODO: send the username and password to server
}

void Scene::fillServerInfo(QString &serverIP, QString &port) {
    QString partUrl = "ws://";
    partUrl += serverIP;
    partUrl += ":";
    partUrl += port;
    qDebug() << partUrl;

    dummy_url = new QUrl(partUrl + "/arena/dummy");
}
