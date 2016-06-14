#include "view.h"

View::View(Scene *scene, QWebSocket &ws) : QGraphicsView(scene), ws(ws) {
    viewWidth = 500;
    viewHeight = 500;
    this->resize(viewWidth, viewHeight);
    this->setWindowTitle("Shape-War");

    setRenderHint(QPainter::Antialiasing, true);
    // setCacheMode(CacheBackground);    // I don't know why but this cause
    // unexpected wrong

    // size control, maybe not necessary
    setMinimumSize(viewWidth, viewHeight);
    setMaximumSize(viewWidth, viewHeight);

    // disable the scroll bar
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    this->setMouseTracking(true); // pretty consumes resources of CPU

    this->self = scene->self;
    this->centerOn(this->self->pos());

    connect(self, SIGNAL(xChanged()), this, SLOT(settingCenter()));
    connect(self, SIGNAL(yChanged()), this, SLOT(settingCenter()));

    sendDelayTimer = new QTimer(this);
    connect(sendDelayTimer, SIGNAL(timeout()), this,
            SLOT(sendControlToServer()));
    sendDelayTimer->start(20);

    key_a_pressed = key_d_pressed = key_s_pressed = key_w_pressed = false;

    expandBtn = new QPushButton("+", this);
    expandBtn->setGeometry(0, viewHeight - buttonLen, buttonLen, buttonLen);
    expandBtn->setVisible(true);
    isExpanded = false;
    connect(expandBtn, SIGNAL(clicked()), this, SLOT(showUpgrateOptions()));

    properties.push_back(new QPair<QLabel *, QPushButton *>(
        new QLabel("Property 1", this), new QPushButton("+", this)));
    properties.push_back(new QPair<QLabel *, QPushButton *>(
        new QLabel("Property 2", this), new QPushButton("+", this)));
    properties.push_back(new QPair<QLabel *, QPushButton *>(
        new QLabel("Property 3", this), new QPushButton("+", this)));
    for (unsigned int i = 0, size = properties.size(); i < size; i++) {
        QPair<QLabel *, QPushButton *> *property = properties.at(i);
        property->first->setGeometry(
            10, viewHeight - (i + 1) * buttonDistance, labelWidth,
            buttonDistance - (buttonDistance - buttonLen));
        property->second->setGeometry(labelWidth + 10,
                                      viewHeight - (i + 1) * buttonDistance,
                                      buttonLen, buttonLen);
        property->first->setVisible(false);
        property->second->setVisible(false);
    }
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

void View::settingCenter() {
    this->centerOn(this->self->pos());
}

void View::mousePressEvent(QMouseEvent *event) {
    mouseClicked = true;
}
void View::mouseReleaseEvent(QMouseEvent *event) {
    mouseClicked = false;
}
void View::mouseMoveEvent(QMouseEvent *event) {
    QPointF mouseP = this->mapToScene(event->pos());
    QPointF selfP = self->pos();

    qreal tangent = (mouseP.y() - selfP.y()) / (mouseP.x() - selfP.x());
    qreal targetAngle = atan(tangent);
    if (mouseP.x() < selfP.x())
        targetAngle += M_PI;
    else if (mouseP.y() < selfP.y())
        targetAngle += M_PI * 2;
    self->setRotation(targetAngle * 180 / 3.14);
    // printf("self (%f, %f), mouse (%f,%f). degree of mouse relative to self
    // is: %f",selfP.x(), selfP.y(), mouseP.x(), mouseP.y(),
    //                  targetAngle * 180.0 / 3.14 );
}
void View::wheelEvent(QWheelEvent *event) {
    // eat this event to prevent it is passed to parent widget
    // do nothing
}

void View::sendControlToServer() {
    // TODO: send all control messages of keyboard/mouse to server

    // debug print
    QJsonObject data;
    data["keys"] = QJsonObject({{"W", key_w_pressed},
                                {"A", key_a_pressed},
                                {"S", key_s_pressed},
                                {"D", key_d_pressed}});
    data["angle"] = self->rotation();
    ws.sendTextMessage(QJsonDocument(data).toJson(QJsonDocument::Compact));
}

void View::showUpgrateOptions() {
    isExpanded = !isExpanded;
    if (isExpanded) {
        expandBtn->setText("-");
        expandBtn->setGeometry(0, viewHeight -
                                      (properties.size() + 1) * buttonDistance,
                               buttonLen, buttonLen);
        for (QPair<QLabel *, QPushButton *> *property : properties) {
            property->first->setVisible(true);
            property->second->setVisible(true);
        }
    } else {
        expandBtn->setText("+");
        expandBtn->setGeometry(0, viewHeight - buttonLen, buttonLen, buttonLen);
        for (QPair<QLabel *, QPushButton *> *property : properties) {
            property->first->setVisible(false);
            property->second->setVisible(false);
        }
    }
}
