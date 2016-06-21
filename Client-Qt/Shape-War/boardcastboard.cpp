#include "boardcastboard.h"
#include <QDebug>

BoardcastBoard::BoardcastBoard(QWidget *parent) : QTextEdit(parent) {
    this->setGeometry(0, 100, this->parentWidget()->width(),
                                 boardcastHeight);
    this->setFocusPolicy(Qt::NoFocus);
    this->setAlignment(Qt::AlignCenter);
    this->setStyleSheet(
        "background-color: rgb(255, 255, 255, 1); border-style: "
        "outset; border-width: 0px; font: normal 30px; color: "
        "rgb(51, 153, 255); text-align: right");
    this->setFont(QFont("monospace"));
    this->setDisabled(true);
    this->setReadOnly(true);
    qDebug() << this->cursor();
    this->setCursor(QCursor(Qt::CrossCursor));
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //this->setView((View)this->parentWidget());
}

void BoardcastBoard::focusOutEvent(QFocusEvent *event) {

}

void BoardcastBoard::focusInEvent(QFocusEvent *event) {

}

void BoardcastBoard::mousePressEvent(QMouseEvent *event) {

}

void BoardcastBoard::mouseReleaseEvent(QMouseEvent *event) {

}

