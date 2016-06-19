#include "chatbar.h"
#include <QDebug>

ChatBar::ChatBar(QString partUrl, QWidget *parent) : QLineEdit(parent) {
    this->chat_url = new QUrl(partUrl + "/chat/roomid");
    this->setStyleSheet("background-color: rgb(255, 0, 0, 1)");
    QObject::connect(&chat_webSocket, &QWebSocket::connected, this,
                     &ChatBar::onConnected);
    this->posY = this->minPosY;
    this->setGeometry(0, this->posY, parentWidth, chatBarHeight);
    this->upTimer = new QTimer(this);
    this->downTimer = new QTimer(this);
    QObject::connect(this->upTimer, SIGNAL(timeout()), this, SLOT(up()));
    QObject::connect(this->downTimer, SIGNAL(timeout()), this, SLOT(down()));
    // chat_webSocket.open(QUrl(*chat_url));
    this->setStyleSheet("background-color: rgba(10, 10, 10, 70); border-style: "
                        "outset; border-width: 0px; font: bold 18px; color: "
                        "rgb(255, 255, 204);");
}

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
        qDebug() << "focus";
    }
}

void ChatBar::sendTextToServer() {

    QString text = this->text();
    QJsonObject data;
    data["name"] = this->name;
    data["message"] = text;
    chat_webSocket.sendTextMessage(
        QJsonDocument(data).toJson(QJsonDocument::Compact));

    this->setText("");
}

void ChatBar::onConnected() {
    qDebug() << "WebSocket of chat connected";
    connect(&chat_webSocket, &QWebSocket::textMessageReceived, this,
            &ChatBar::onTextMessageReceived);
}

void ChatBar::onTextMessageReceived(QString message) {
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    const auto &object = doc.object();

    // TODO: display chat message from server
}

void ChatBar::focusInEvent(QFocusEvent *) {
}

void ChatBar::focusOutEvent(QFocusEvent *) {
}

void ChatBar::setParentWidth(int width) {
    this->parentWidth = width;
    this->setGeometry(0, this->posY, parentWidth, chatBarHeight);
}

void ChatBar::up() {
    if (this->posY == this->minPosY) {
        this->upTimer->stop();
        return;
    }
    --this->posY;
    this->setGeometry(0, this->posY, parentWidth, chatBarHeight);
}

void ChatBar::down() {
    if (this->posY == this->maxPosY) {
        this->downTimer->stop();
        return;
    }
    ++this->posY;
    this->setGeometry(0, this->posY, parentWidth, chatBarHeight);
}

void ChatBar::setName(QString name) {
    this->name = name;
}
