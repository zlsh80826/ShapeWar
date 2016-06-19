#include "chatbar.h"
#include <QDebug>

ChatBar::ChatBar(QString partUrl, QWidget * parent) : QLineEdit(parent) {
    this->chat_url = new QUrl( partUrl + "/chat/roomid" );
    this->setStyleSheet("background-color: rgb(255, 0, 0, 1)");
    QObject::connect(&chat_webSocket, &QWebSocket::connected, this, &ChatBar::onConnected);
    this->posY = this->minPosY;
    this->setGeometry(0, this->posY, parentWidth, chatBarHeight);
    this->upTimer = new QTimer(this);
    this->downTimer = new QTimer(this);
    QObject::connect(this->upTimer, SIGNAL(timeout()), this, SLOT(ChatBar::up()));
    QObject::connect(this->downTimer, SIGNAL(timeout()), this, SLOT(ChatBar::down()));
    //chat_webSocket.open(QUrl(*chat_url));
}

void ChatBar::startChat() {
    if(this->hasFocus()) {
        this->clearFocus();
        this->upTimer->start(20);
        qDebug() << "!focus";
        this->sendTextToServer();
    } else {
        this->setFocus();
        this->downTimer->start(20);
        qDebug() << "focus";
    }
}

void ChatBar::sendTextToServer()
{
    // TODO: send text in chat bar to server

    // reset text in chat bar
    this->setText("");
}

void ChatBar::onConnected()
{
    qDebug() << "WebSocket of chat connected";
    connect(&chat_webSocket, &QWebSocket::textMessageReceived, this,
            &ChatBar::onTextMessageReceived);
}

void ChatBar::onTextMessageReceived(QString message)
{
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
    qDebug() << "up";
    if( this->posY == this->minPosY ) {
        this->upTimer->stop();
        return;
    }
    --this->posY;
    this->setGeometry(0, this->posY, parentWidth, chatBarHeight);
}

void ChatBar::down() {
    qDebug() << "down";
    if( this->posY == this->maxPosY ) {
        this->downTimer->stop();
        return;
    }
    ++this->posY;
    this->setGeometry(0, this->posY, parentWidth, chatBarHeight);
}
