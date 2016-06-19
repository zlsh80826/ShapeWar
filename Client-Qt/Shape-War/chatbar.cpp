#include "chatbar.h"
#include <QDebug>

ChatBar::ChatBar(QWidget * parent) : QLineEdit(parent){

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

void ChatBar::focusInEvent(QFocusEvent *) {
    this->setVisible(true);
    // ^^^^^ still not needed?
}

void ChatBar::focusOutEvent(QFocusEvent *) {
    this->setVisible(false);
    // ^^^^^ still not needed?
}
