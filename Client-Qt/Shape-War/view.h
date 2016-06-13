#ifndef VIEW_H
#define VIEW_H

#include "scene.h"
#include "hero.h"
#include <QGraphicsView>
#include <QKeyEvent>
#include <QDebug>
#include <QTimer>

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(Scene *scene);

public slots:
    void settingCenter();
    void sendControlToServer();

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
};

#endif // VIEW_H
