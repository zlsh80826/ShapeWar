#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QColor>
#include <hero.h>
#include <QKeyEvent>

class Scene : public QGraphicsView
{
    Q_OBJECT

public:
    Scene(QWidget* parent = 0);

public slots:
    void startGame();
    void gameOver();

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    Hero* self;
    void initView();
    void initGame();
    int width;
    int height;
    int margin;

    bool key_w_pressed;
    bool key_a_pressed;
    bool key_s_pressed;
    bool key_d_pressed;

    bool mouseLeftClicked;
    bool mouseRightClicked;


};

#endif // SCENE_H
