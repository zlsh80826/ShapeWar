#include "scene.h"
#include "hero.h"
#include "logindialog.h"
#include "polygon.h"
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

    cleanTimer = 0;
}
void Scene::print_freq() {
    qDebug() << "recv per sec: " << this->recvs;
    this->recvs = 0;
}
void Scene::drawBackground(QPainter *painter, const QRectF &rect) {
    (void)rect;
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
    this->self->setZValue(2);
    this->self->bullets->addToParentNoHPBar(this);

    // test name
    this->self->info->setName("test");
    this->addItem(this->self->info);

    // test monster
    triangles = new PolygonGroup<Polygon>(3, 50);
    triangles->addToParent(this);

    rectangles = new PolygonGroup<Polygon>(4, 250);
    rectangles->addToParent(this);

    pentagons = new PolygonGroup<Polygon>(5, 10);
    pentagons->addToParent(this);
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

    cleanTimer++;
    if (cleanTimer >= 400) {
        cleanTimer = 0;
        cleanHit.clear();
    }
    for (const auto &hero_data : object["players"].toArray()) {
        const auto &hero_object = hero_data.toObject();
        int heroId = hero_object["id"].toInt();
        if (hero_object["id"] != self_id) {
            Hero *hero;
            if (not this->heroes.contains(heroId)) {
                hero = new Hero;
                this->heroes[heroId] = hero;
                this->addItem(hero);
                this->addItem(hero->hpBar);
                hero->bullets->addToParentNoHPBar(this);
            } else {
                hero = this->heroes[heroId];
            }
            hero->read_player(hero_object);
        }
        if (cleanTimer == 0) {
            cleanHit.insert(heroId);
        }
    }
    if (cleanTimer == 0) {
        QHash<int, Hero *>::iterator iter = heroes.begin(), end = heroes.end();
        QHash<int, Hero *>::iterator prev;
        while (iter != end) {
            prev = iter++;
            if (cleanHit.contains(prev.key()) == false) {
                this->removeItem(prev.value());
                delete prev.value();
                heroes.erase(prev);
            }
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
