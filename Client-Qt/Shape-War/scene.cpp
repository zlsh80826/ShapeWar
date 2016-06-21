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

/*!
 * \brief Scene::Scene constructor
 * \param parent
 */
Scene::Scene(QWidget *parent) : QGraphicsScene(parent) {
    // new login dialog and connect accept login signal and slot
    LoginDialog *loginDialog = new LoginDialog();

    QObject::connect(
        loginDialog, SIGNAL(acceptLogin(QString &, QString &,
                                        QString &)), //, QString &, bool)),
        this,
        SLOT(slotAcceptUserLogin(QString &, QString &,
                                 QString &))); // QString &, bool)));
    loginDialog->exec();

    this->width = 5000;
    this->height = 4000;
    this->margin = 10;
    initGame();
    startGame();

    // connent signals and slots and start webSocket
    connect(&m_webSocket, &QWebSocket::connected, this, &Scene::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &Scene::closed);
    m_webSocket.open(QUrl(*dummy_url));

    // debug and test transmittion rating
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
/*!
 * \brief Scene::drawBackground draws the background of scene
 * \param painter
 * \param rect
 */
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

// we already start game at server, so this function is abandoned
void Scene::startGame() {

}

/*!
 * \brief Scene::initGame initial the self and other heros and containers of monstors
 */
void Scene::initGame() {

    this->setSceneRect(5, 5, width, height);
    this->backgroundColor = QColor(10, 10, 255, 30);

    self = new Self();
    self->setPos(100, 200);
    this->addItem(self);
    this->addItem(self->hpBar);
    this->self->setZValue(2);
    this->self->bullets->addToParentNoHPBar(this);

    // test name
    this->self->info->setName(username);
    this->addItem(this->self->info);

    // test monster
    triangles = new PolygonGroup<Polygon>(3, 50);
    triangles->addToParent(this);

    rectangles = new PolygonGroup<Polygon>(4, 250);
    rectangles->addToParent(this);

    pentagons = new PolygonGroup<Polygon>(5, 10);
    pentagons->addToParent(this);
}

// the game never over ( unless the server is corrupt )
void Scene::gameOver() {
}

/*!
 * \brief Scene::onConnected when connected, connect signal/slot when message is received from server
 */
void Scene::onConnected() {
    qDebug() << "WebSocket of arena connected";
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived, this,
            &Scene::onBinaryMessageReceived);
}

/*!
 * \brief Scene::onBinaryMessageReceived uncompress the date and tranlate is using UTF8
 * \param data
 */
void Scene::onBinaryMessageReceived(QByteArray data) {
    onTextMessageReceived(QString::fromUtf8(qUncompress(data)));
}

/*!
 * \brief Scene::onTextMessageReceived interpret uncompressed data to Json
 * and make objects in all containers to read their information
 * \param message
 */
void Scene::onTextMessageReceived(QString message) {
    // qDebug().noquote() << "Message received:" << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    const auto &object = doc.object();

    if (object.contains("self")) {
        self_id = object["self"].toObject()["id"].toInt(-1);
        this->self->read_info(object);
    } else if (self_id != -1) {
        this->recvs++;
        this->triangles->read(object["triangles"].toArray());
        this->rectangles->read(object["squares"].toArray());
        this->pentagons->read(object["pentagons"].toArray());

        cleanTimer++;
        if (cleanTimer >= 400) {
            cleanTimer = 0;
            cleanHit.clear();
        }

        for (const auto &hero_data : object["players"].toArray()) {
            const auto &hero_object = hero_data.toObject();
            int heroId = hero_object["id"].toInt();
            if (hero_object["id"].toInt() != self_id) {
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
            } else {
                self->read_global(hero_object);
            }
            if (cleanTimer == 0) {
                cleanHit.insert(heroId);
            }
        }
        if (cleanTimer == 0) {
            QHash<int, Hero *>::iterator iter = heroes.begin(),
                                         end = heroes.end();
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
}

/*!
 * \brief Scene::slotAcceptUserLogin fill necesary information
 * \param serverIP
 * \param port
 * \param username
 */
void Scene::slotAcceptUserLogin(QString &serverIP, QString &port,
                                QString &username) {
    // QString &password, bool isAnonymous) {
    partUrl = QString("ws://");
    partUrl += serverIP;
    partUrl += ":";
    partUrl += port;
    dummy_url = new QUrl(partUrl + "/arena/dummy");
    this->username = username;
    // test
    qDebug() << "Get serverIP: " << serverIP << ", port: " << port
             << ", username: " << username;
    // TODO: send the username and password to server
    // ^^^ don't need it anymore
}

/*!
 * \brief Scene::getPartUrl getter of partial url
 * \return
 */
QString Scene::getPartUrl() const {
    return partUrl;
}

/*!
 * \brief Scene::getUsername getter of user name
 * \return
 */
QString Scene::getUsername() const {
    return username;
}
