#include "chatbar.h"
#include <QDebug>

ChatBar::ChatBar(QString partUrl, QWidget * parent) : QLineEdit(parent){
    this->chat_url = new QUrl( partUrl + "/chat/roomid" );
    this->clearFocus();
    this->setVisible(false);

    connect(&chat_webSocket, &QWebSocket::connected, this, &ChatBar::onConnected);
    //chat_webSocket.open(QUrl(*chat_url));
}

void ChatBar::startChat()
{
    if(this->hasFocus()) {
        this->clearFocus();
        this->setVisible(false);
        this->sendTextToServer();
    } else {
        this->setVisible(true);
        this->setFocus();
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
    this->setVisible(true);
}

void ChatBar::focusOutEvent(QFocusEvent *) {
    this->setVisible(false);
}
