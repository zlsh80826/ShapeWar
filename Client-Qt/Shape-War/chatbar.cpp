#include "chatbar.h"
#include <QDebug>

ChatBar::ChatBar(QWidget * parent) : QLineEdit(parent){

}

void ChatBar::focusInEvent(QFocusEvent *event) {
    this->setVisible(true);
}

void ChatBar::focusOutEvent(QFocusEvent *event) {
    this->setVisible(false);
}
