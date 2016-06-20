#include "chatbar.h"
#include <QDebug>

ChatBar::ChatBar(QString partUrl, QWidget *parent) : QLineEdit(parent) {
    this->chat_url = new QUrl(partUrl + "/chat");
    this->setStyleSheet("background-color: rgb(255, 0, 0, 1)");

    this->posY = this->minPosY;
    this->setGeometry(0, this->posY, this->parentWidget()->width(),
                      chatBarHeight);
    this->upTimer = new QTimer(this);
    this->downTimer = new QTimer(this);
    QObject::connect(this->upTimer, SIGNAL(timeout()), this, SLOT(up()));
    QObject::connect(this->downTimer, SIGNAL(timeout()), this, SLOT(down()));
    this->setStyleSheet("background-color: rgba(10, 10, 10, 70); border-style: "
                        "outset; border-width: 0px; font: bold 18px; color: "
                        "rgb(255, 255, 204);");
    QObject::connect(&chat_webSocket, &QWebSocket::connected, this,
                     &ChatBar::onConnected);
    chat_webSocket.open(QUrl(*chat_url));
    this->boardcast = new QTextEdit(this->parentWidget());
    this->boardcast->setGeometry(0, 100, this->parentWidget()->width(),
                                 boardcastHeight);
    // this->boardcast->setFocusPolicy(Qt::NoFocus);
    this->boardcast->setAlignment(Qt::AlignCenter);
    this->boardcast->setStyleSheet(
        "background-color: rgba(0, 0, 0, 2); border-style: "
        "outset; border-width: 0px; font: normal 30px; color: "
        "rgb(51, 153, 255); text-align: right");
    this->boardcast->setFont(QFont("monospace"));
    this->boardcast->setDisabled(true);
    this->boardcast->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->boardcast->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->clearTimer = new QTimer(this);
    QObject::connect(this->clearTimer, SIGNAL(timeout()), this,
                     SLOT(clearTimeoutContent()));
    clearTimer->start(3000);
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
    QString sender = object["name"].toString();
    QString content = object["message"].toString();
    qDebug() << sender << ": " << content;
    this->boardcastContent.append(sender + ": " + content);
    this->boardcast->setText("");
    this->boardcast->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < boardcastContent.size(); ++i) {
        this->boardcast->append(boardcastContent[i]);
    }
}

void ChatBar::focusInEvent(QFocusEvent *) {
    qDebug() << "in";
}

void ChatBar::focusOutEvent(QFocusEvent *) {
    qDebug() << "out";
}

void ChatBar::mousePressEvent(QMouseEvent *event) {
    qDebug() << "asd";
}

void ChatBar::setParentWidth() {
    this->setGeometry(0, this->posY, this->parentWidget()->width(),
                      chatBarHeight);
    this->boardcast->setGeometry(0, 100, this->parentWidget()->width(),
                                 boardcastHeight);
}

void ChatBar::up() {
    if (this->posY == this->minPosY) {
        this->upTimer->stop();
        return;
    }
    --this->posY;
    this->setGeometry(0, this->posY, this->parentWidget()->width(),
                      chatBarHeight);
}

void ChatBar::down() {
    if (this->posY == this->maxPosY) {
        this->downTimer->stop();
        return;
    }
    ++this->posY;
    this->setGeometry(0, this->posY, this->parentWidget()->width(),
                      chatBarHeight);
}

void ChatBar::setName(QString name) {
    this->name = name;
}

void ChatBar::clearTimeoutContent() {
    if (!this->boardcastContent.isEmpty()) {
        if (this->boardcastContent.size() > 3)
            this->boardcastContent.resize(3);
        this->boardcastContent.pop_front();
    }
    this->boardcast->setText("");
    this->boardcast->setAlignment(Qt::AlignCenter);
    for (int i = 0; i < boardcastContent.size(); ++i) {
        this->boardcast->append(boardcastContent[i]);
    }
}
