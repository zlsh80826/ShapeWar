#include "view.h"
View::View(Scene *scene, QWebSocket &ws) : QGraphicsView(scene), ws(ws) {
    this->resize(viewWidth, viewHeight);
    this->setWindowTitle("Shape-War");

    setRenderHint(QPainter::Antialiasing, true);
    // setCacheMode(CacheBackground);    // I don't know why but this cause
    // unexpected wrong

    // size control, maybe not necessary
    setMinimumSize(500, 400);
    setMaximumSize(maxViewWidth, maxViewHeight);

    // disable the scroll bar
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setMouseTracking(true); // pretty consumes resources of CPU

    this->self = scene->self;
    this->centerOn(this->self->pos());

    connect(self, SIGNAL(xChanged()), this, SLOT(onSelfPosChanged()));
    connect(self, SIGNAL(yChanged()), this, SLOT(onSelfPosChanged()));

    sendDelayTimer = new QTimer(this);
    connect(sendDelayTimer, SIGNAL(timeout()), this,
            SLOT(sendControlToServer()));
    sendDelayTimer->start(20);

    key_a_pressed = key_d_pressed = key_s_pressed = key_w_pressed = false;
    mouseClicked = false;

    expandBtn = new QPushButton("+", this);
    expandBtn->setGeometry(0, viewHeight - buttonLen, buttonLen, buttonLen);
    expandBtn->setVisible(true);
    isExpanded = false;
    connect(expandBtn, SIGNAL(clicked()), this, SLOT(showUpgrateOptions()));

    for (int i = 0, size = self->passiveNames.size(); i < size; i++) {
        properties.push_back(new QPair<QLabel *, QPushButton *>(
            new QLabel(self->passiveNames.at(i), this),
            new QPushButton("+", this)));
    }

    propertyBtnPtrGroup = new QButtonGroup(this);
    for (unsigned int i = 0, size = properties.size(); i < size; i++) {
        QPair<QLabel *, QPushButton *> *property = properties.at(i);
        property->first->setGeometry(
            10, viewHeight - (i + 1) * passiveDistance, labelWidth,
            passiveDistance - (passiveDistance - passiveHeight));
        property->second->setGeometry(labelWidth + 10,
                                      viewHeight - (i + 1) * passiveDistance,
                                      buttonLen + 10, passiveHeight);
        property->first->setVisible(false);
        property->second->setVisible(false);
        // property->first->setStyleSheet("background-color : yellow; color :
        // white");

        propertyBtnPtrGroup->addButton(property->second, i);
    }
    this->setPropertyStyle();
    connect(self, SIGNAL(upgradePointsChanged()), this,
            SLOT(onUpgradePointChanged()));
    connect(propertyBtnPtrGroup, SIGNAL(buttonClicked(int)), this,
            SLOT(onPropertyBtnClicked(int)));

    sec = new QTimer(0);
    this->sends = 0;
    connect(sec, SIGNAL(timeout()), this, SLOT(print_freq()));
    sec->start(1000);

}
void View::print_freq() {
    qDebug() << "send per sec: " << this->sends;
    this->sends = 0;
}

void View::keyPressEvent(QKeyEvent *event) {
    // ment is just for testing
    // whether to move to judged by server instead of here
    const qreal moveAmount = 3.0;

    switch (event->key()) {
    case Qt::Key_W:
        this->key_w_pressed = true;
        break;
    case Qt::Key_A:
        this->key_a_pressed = true;
        break;
    case Qt::Key_S:
        this->key_s_pressed = true;
        break;
    case Qt::Key_D:
        this->key_d_pressed = true;
        break;
    }
}

void View::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) {
        event->ignore();
        return;
    }
    switch (event->key()) {
    case Qt::Key_W:
        this->key_w_pressed = false;
        break;
    case Qt::Key_A:
        this->key_a_pressed = false;
        break;
    case Qt::Key_S:
        this->key_s_pressed = false;
        break;
    case Qt::Key_D:
        this->key_d_pressed = false;
        break;
    }
    // qDebug() << "Released key: " << event->key();
}

void View::onSelfPosChanged() {
    this->centerOn(this->self->pos());
    QPointF mouseP = this->mapToScene(this->mapFromGlobal(QCursor::pos()));
    self->setRotation(this->calcRargetAngle(mouseP));
    self->setInfoPos(this->mapToScene(QPoint(InfoCenterX, InfoCenterY)));
}

void View::mousePressEvent(QMouseEvent *event) {
    mouseClicked = true;
}
void View::mouseReleaseEvent(QMouseEvent *event) {
    mouseClicked = false;
}
void View::mouseMoveEvent(QMouseEvent *event) {
    QPointF mouseP = this->mapToScene(event->pos());
    self->setRotation(this->calcRargetAngle(mouseP));
    // printf("self (%f, %f), mouse (%f,%f). degree of mouse relative to self
    // is: %f",selfP.x(), selfP.y(), mouseP.x(), mouseP.y(),
    //                  targetAngle * 180.0 / 3.14 );
}
qreal View::calcRargetAngle(QPointF &mouseP) {
    QPointF selfP = self->pos();
    qreal tangent = (mouseP.y() - selfP.y()) / (mouseP.x() - selfP.x());
    qreal targetAngle = atan(tangent);
    if (mouseP.x() < selfP.x())
        targetAngle += M_PI;
    else if (mouseP.y() < selfP.y())
        targetAngle += M_PI * 2;
    targetAngle *= 180;
    targetAngle /= 3.14;
    return targetAngle;
}

void View::wheelEvent(QWheelEvent *event) {
    // eat this event to prevent it is passed to parent widget
    // do nothing

    // below are just testing other things
    printf("From %d to", self->getUpgradePoints());
    if (event->orientation() == Qt::Vertical) {
        if (event->delta() > 0) {
            self->setUpgradePoints(self->getUpgradePoints() + 1);
        } else {
            self->setUpgradePoints(self->getUpgradePoints() - 1);
        }
    }
    printf(" %d", self->getUpgradePoints());
}

void View::resizeEvent(QResizeEvent *event)
{
    int nowViewWidth = this->width();
    int nowViewHeight = this->height();
    expandBtn->setGeometry(0, nowViewHeight - buttonLen, buttonLen, buttonLen);
    for (unsigned int i = 0, size = properties.size(); i < size; i++) {
        QPair<QLabel *, QPushButton *> *property = properties.at(i);
        property->first->setGeometry(
            10, nowViewHeight - (i + 1) * passiveDistance, labelWidth,
            passiveHeight);
        property->second->setGeometry(labelWidth + 10,
                                      nowViewHeight - (i + 1) * passiveDistance,
                                      buttonLen + 10, passiveHeight);
    }
    InfoCenterX = nowViewWidth/2;
    InfoCenterY = nowViewHeight - InfoHeightOffset;
    self->setInfoPos(this->mapToScene(QPoint(InfoCenterX, InfoCenterY)));
    this->centerOn(self);
    self->info->update(this->rect());
}

void View::sendControlToServer() {
    this->sends++;
    QJsonObject data;
    data["keys"] = QJsonObject({{"W", key_w_pressed},
                                {"A", key_a_pressed},
                                {"S", key_s_pressed},
                                {"D", key_d_pressed}});
    data["mouse"] = mouseClicked;
    data["angle"] = self->rotation();
    ws.sendTextMessage(QJsonDocument(data).toJson(QJsonDocument::Compact));
}

void View::showUpgrateOptions() {
    isExpanded = !isExpanded;
    if (isExpanded) {
        expandBtn->setText("-");
        expandBtn->setGeometry(0, this->height() -
                                      (properties.size() + 1) * buttonDistance,
                               buttonLen, buttonLen);
        for (QPair<QLabel *, QPushButton *> *property : properties) {
            property->first->setVisible(true);
            property->second->setVisible(true);
        }
    } else {
        expandBtn->setText("+");
        expandBtn->setGeometry(0, this->height() - buttonLen, buttonLen, buttonLen);
        for (QPair<QLabel *, QPushButton *> *property : properties) {
            property->first->setVisible(false);
            property->second->setVisible(false);
        }
    }
}

void View::onUpgradePointChanged() {
    bool enanbled = (self->getUpgradePoints() > 0) ? true : false;
    for (QPair<QLabel *, QPushButton *> *property : properties) {
        property->first->setEnabled(enanbled);
        property->second->setEnabled(enanbled);
    }
}

void View::onPropertyBtnClicked(int clickedBtnId) {
    qDebug() << "Button clicked: " << this->self->passiveNames.at(clickedBtnId);
    // TODO: handle the message want to  pass to server
}

void View::setPropertyStyle() {
    for (int i = 0; i < properties.size(); ++i) {
        this->properties.at(i)->first->setStyleSheet(
            "background-color: rgb(61, 61, 61, 240); border-style: outset; "
            "border-color: rgb(61, 61, 61, 240); border-top-left-radius: "
            "10px; border-bottom-left-radius: 10px; font: bold 14px; "
            "text-align: right; color: rgb(218, 218, 218, 240);");
        this->properties.at(i)->first->setAlignment(Qt::AlignCenter);
    }
    QString common =
        "border-top-right-radius:10px; border-bottom-right-radius: 10px; "
        "font: bold 14px; border-width: 2px; border-style: outset; "
        "border-color: rgb(61, 61, 61, 240); color: rgb(61, 61, 61, 240);";
    this->properties.at(0)->second->setStyleSheet(
        common.append("background-color: rgb(108, 240, 236, 255);"));
    this->properties.at(1)->second->setStyleSheet(
        common + "background-color: rgb(152, 240, 108, 255);");
    this->properties.at(2)->second->setStyleSheet(
        common + "background-color: rgb(240, 108, 108, 255);");
    this->properties.at(3)->second->setStyleSheet(
        common + "background-color: rgb(240, 217, 108, 255);");
    this->properties.at(4)->second->setStyleSheet(
        common + "background-color: rgb(108, 150, 240, 255);");
    this->properties.at(5)->second->setStyleSheet(
        common + "background-color: rgb(154, 108, 240, 255);");
    this->properties.at(6)->second->setStyleSheet(
        common + "background-color: rgb(236, 108, 240, 255);");
    this->properties.at(7)->second->setStyleSheet(
        common + "background-color: rgb(238, 182, 143, 255);");
}
