#ifndef VIEW_H
#define VIEW_H

#include "scene.h"
#include "hero.h"
#include <QGraphicsView>
#include <QKeyEvent>

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(Scene *scene);

public slots:
    void settingCenter();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

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
    QTimer *recvDelayTimer;
};

#endif // VIEW_H
