#ifndef VIEW_H
#define VIEW_H

#include "scene.h"
#include "hero.h"
#include <QGraphicsView>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QWebSocket>
#include <triangle.h>
#include <rectangle.h>
#include <pentagon.h>

#include <QPushButton>
#include <QLabel>
#include <QPair>

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(Scene *scene, QWebSocket& ws);

public slots:
    void settingCenter();
    void sendControlToServer();
    void showUpgrateOptions();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    int viewWidth, viewHeight;
    Hero *self;

    bool key_w_pressed;
    bool key_a_pressed;
    bool key_s_pressed;
    bool key_d_pressed;

    bool mouseClicked;
    qreal targetAngle;

    QTimer *sendDelayTimer;
    const int sendDelay = 500;

    // for upgrade option
    const int buttonLen = 30;
    const int buttonDistance = 33;
    const int labelWidth = 100;
    QPushButton *expandBtn;
    bool isExpanded;
    QVector<QPair<QLabel *, QPushButton *>* > properties;
    QWebSocket& ws;
};

#endif // VIEW_H
