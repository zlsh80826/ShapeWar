#include "boardcastboard.h"
#include <QDebug>

BoardcastBoard::BoardcastBoard(QWidget *parent) : QTextEdit(parent) {
    // set location and size
    this->setGeometry(0, 100, this->parentWidget()->width(),
                                 boardcastHeight);
    // never focus to this widget
    this->setFocusPolicy(Qt::NoFocus);

    // make text to align at center
    this->setAlignment(Qt::AlignCenter);

    // the style (background, border, font, color, ...etc) of this widget
    this->setStyleSheet(
        "background-color: rgb(255, 255, 255, 1); border-style: "
        "outset; border-width: 0px; font: normal 30px; color: "
        "rgb(51, 153, 255); text-align: right");
    this->setFont(QFont("monospace"));

    // disable widget (make it not handle keyboard/mouse event)
    this->setDisabled(true);
    // the text is read only
    this->setReadOnly(true);
    qDebug() << this->cursor();
    // set cursor
    this->setCursor(QCursor(Qt::CrossCursor));
    // no scroll
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void BoardcastBoard::focusOutEvent(QFocusEvent *) {
    // qDebug() << " board focus out";
}

void BoardcastBoard::focusInEvent(QFocusEvent *) {
    // qDebug() << " board focus in";
}

void BoardcastBoard::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void BoardcastBoard::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

