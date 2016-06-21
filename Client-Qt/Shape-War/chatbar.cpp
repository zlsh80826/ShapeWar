#include "chatbar.h"
#include <QDebug>

/*!
 * \brief ChatBar::ChatBar constructor
 * \param partUrl the partial url provided by user when typing the server IP, port
 * \param parent the widget that wants to be this object's parent
 */
ChatBar::ChatBar(QString partUrl, QWidget *parent) : QLineEdit(parent) {
    // set up the full url of chat service
    this->chat_url = new QUrl(partUrl + "/chat");
    this->setStyleSheet("background-color: rgb(255, 0, 0, 1)");

    // set up the location, size of chat bar
    this->posY = this->minPosY;
    this->setGeometry(0, this->posY, this->parentWidget()->width(),
                      chatBarHeight);

    // new timer of up/down animation and connect them to the function to animate
    this->upTimer = new QTimer(this);
    this->downTimer = new QTimer(this);
    QObject::connect(this->upTimer, SIGNAL(timeout()), this, SLOT(up()));
    QObject::connect(this->downTimer, SIGNAL(timeout()), this, SLOT(down()));

    // set the style (background, border, font, color, ...etc) of this widget
    this->setStyleSheet("background-color: rgba(10, 10, 10, 70); border-style: "
                        "outset; border-width: 0px; font: bold 18px; color: "
                        "rgb(255, 255, 204);");

    // when this web socket is connected, do necessary things on ChatBar::onConnected
    QObject::connect(&chat_webSocket, &QWebSocket::connected, this,
                     &ChatBar::onConnected);
    // start connect to server's chat service
    chat_webSocket.open(QUrl(*chat_url));

    // new broadcast board
    this->broadcast = new BoardcastBoard(this->parentWidget());
    // new clear timer, which clear one broadcast message on screen
    // and append one that is not on screen yet ( if any ) when timeout
    this->clearTimer = new QTimer(this);
    QObject::connect(this->clearTimer, SIGNAL(timeout()), this,
                     SLOT(clearTimeoutContent()));
    clearTimer->start(3000);
}

/*!
 * \brief If the player set its focus into chat bar (by typing enter), animate the process of bar going down.
 * If the player set its focus out from chat bar by typing enter, animate bar going up and
 * send the chat message to server
 */
void ChatBar::startChat() {
    if (this->hasFocus()) {
        this->clearFocus();
        if (this->downTimer->isActive()) {
            this->downTimer->stop();
        }
        this->upTimer->start(10);
        this->sendTextToServer();
    } else {
        this->setFocus();
        if (this->upTimer->isActive()) {
            this->upTimer->stop();
        }
        this->downTimer->start(10);
    }
}

/*!
 * \brief ChatBar::sendTextToServer implement the sending process of chat message to server
 */
void ChatBar::sendTextToServer() {

    QString text = this->text();
    if(text == "")
        return;
    QJsonObject data;
    data["name"] = this->name;
    data["message"] = text;
    chat_webSocket.sendTextMessage(
        QJsonDocument(data).toJson(QJsonDocument::Compact));

    this->setText("");
}

/*!
 * \brief when chat's webSocket connect to server successsively, connect
 *  the slot to handle chat message of other players from server
 */
void ChatBar::onConnected() {
    qDebug() << "WebSocket of chat connected";
    connect(&chat_webSocket, &QWebSocket::textMessageReceived, this,
            &ChatBar::onTextMessageReceived);
}

/*!
 * \brief ChatBar::onTextMessageReceived is called when new chat message of other players is received from server.
 * It will paaend the message to broadcast board
 * \param message
 */
void ChatBar::onTextMessageReceived(QString message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    const auto &object = doc.object();

    // TODO: display chat message from server
    QString sender = object["name"].toString();
    QString content = object["message"].toString();
    qDebug() << sender << ": " << content;
    this->broadcastContent.append(sender + ": " + content);
    this->broadcast->setText("");
    this->broadcast->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < broadcastContent.size(); ++i) {
        this->broadcast->append(broadcastContent[i]);
    }
}

// for dubugging
void ChatBar::focusInEvent(QFocusEvent *) {
    qDebug() << "in";
}
void ChatBar::focusOutEvent(QFocusEvent *) {
    qDebug() << "out";
}

/*!
 * \brief ChatBar::setParentWidth set the location and size of chat bar and broadcast board
 */
void ChatBar::setParentWidth() {
    this->setGeometry(0, this->posY, this->parentWidget()->width(),
                      chatBarHeight);
    this->broadcast->setGeometry(0, 100, this->parentWidget()->width(),
                                 broadcastHeight);
}

/*!
 * \brief ChatBar::up implements the animation of bar's going up
 */
void ChatBar::up() {
    if (this->posY == this->minPosY) {
        this->upTimer->stop();
        return;
    }
    --this->posY;
    this->setGeometry(0, this->posY, this->parentWidget()->width(),
                      chatBarHeight);
}
/*!
 * \brief ChatBar::down implements the animation of bar's going down
 */
void ChatBar::down() {
    if (this->posY == this->maxPosY) {
        this->downTimer->stop();
        return;
    }
    ++this->posY;
    this->setGeometry(0, this->posY, this->parentWidget()->width(),
                      chatBarHeight);
}

/*!
 * \brief ChatBar::setName set the name of player (using the information player input when login)
 * \param name
 */
void ChatBar::setName(QString name) {
    this->name = name;
}

/*!
 * \brief ChatBar::clearTimeoutContent update content on broadcastContent
 * and make broadcast board Synchronize with it.
 */
void ChatBar::clearTimeoutContent() {
    if (!this->broadcastContent.isEmpty()) {
        if (this->broadcastContent.size() > 3)
            this->broadcastContent.resize(3);
        this->broadcastContent.pop_front();
    }
    this->broadcast->setText("");
    this->broadcast->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < broadcastContent.size(); ++i) {
        this->broadcast->append(broadcastContent[i]);
    }
}

