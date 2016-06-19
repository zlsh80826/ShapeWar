#include "selfinfo.h"
#include <QDebug>
#include <QFont>
#include <QPainter>
#include <QTimer>

SelfInfo::SelfInfo() {
    this->score = 0;
    this->targetScore = 0;
    this->maxScore = 50000;
    this->lv = 1;
    this->exp = 0;
    this->targetExp = 0;
    this->expWidth = 0;
    this->maxExpWidth = 600;
    this->scoreWidth = 0;
    this->maxScoreWidth = 500;
    this->setOpacity(0.95);
    this->setZValue(1);
    this->expTimer = new QTimer(this);
    this->scoreTimer = new QTimer(this);
    QObject::connect(this->expTimer, SIGNAL(timeout()), this, SLOT(expAni()));
    QObject::connect(this->scoreTimer, SIGNAL(timeout()), this,
                     SLOT(scoreAni()));
}

void SelfInfo::setName(QString name) {
    this->name = name;
}

QRectF SelfInfo::boundingRect() const {
    return QRectF(-380, -40, 760, 160);
}

void SelfInfo::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget) {
    (void)option;
    (void)widget;

    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor(85, 85, 85, 240));
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(pen);
    painter->setFont(QFont("sansaerif", 40, QFont::Bold));
    painter->drawText(-50, 30, name);
    pen.setColor(QColor(61, 61, 61, 255));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(61, 61, 61, 255), Qt::SolidPattern));
    painter->drawRect(QRect(-maxScoreWidth / 2, 50, maxScoreWidth, 20));
    painter->setBrush(QBrush(QColor(108, 240, 162, 255), Qt::SolidPattern));
    painter->drawRect(QRect(-maxScoreWidth / 2, 50, scoreWidth, 20));
    painter->setBrush(QBrush(QColor(61, 61, 61, 255), Qt::SolidPattern));
    painter->drawRect(QRect(-maxExpWidth / 2, 80, maxExpWidth, 20));
    painter->setBrush(QBrush(QColor(240, 217, 108, 255), Qt::SolidPattern));
    painter->drawRect(QRect(-maxExpWidth / 2, 80, this->expWidth, 20));
}

void SelfInfo::setExp(int new_exp) {
    if (new_exp == this->targetExp)
        return;
    if (this->expTimer->isActive()) {
        this->expTimer->setInterval(5);
        return;
    }
    this->targetExp = new_exp;
    this->expTimer->start(20);
}

void SelfInfo::setScore(int new_score) {
    if (new_score == this->targetScore)
        return;
    if (this->scoreTimer->isActive()) {
        this->scoreTimer->setInterval(5);
        return;
    }
    this->targetScore = new_score;
    this->scoreTimer->start(20);
}

void SelfInfo::setLv(int new_lv) {
    this->lv = new_lv;
}

void SelfInfo::expAni() {
    if (this->targetExp == this->exp ) {
        ++this->exp ;
        expTimer->stop();
        return;
    }
    if (this->targetExp > this->exp + 10) {
        exp += (targetExp-this->exp) / 10;
    } else {
        ++this->exp;
    }
    this->expWidth = (this->maxExpWidth * this->exp) / (this->lv * 300);
    this->update(this->boundingRect());
}

void SelfInfo::scoreAni() {
    if (this->targetScore == this->score) {
        scoreTimer->stop();
        return;
    }
    if (this->targetScore > this->score) {
        ++this->score;
    }
    this->scoreWidth = (this->maxScoreWidth * this->score) / this->maxScore;
    this->update(this->boundingRect());
}
