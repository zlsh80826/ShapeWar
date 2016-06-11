#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QColor>
#include <hero.h>

class Scene : public QGraphicsView
{
    Q_OBJECT

public:
    Scene(QWidget* parent = 0);

public slots:
    void startGame();
    void gameOver();

protected:
    //void mousePressEvent(QMouseEvent *event);
    //void keyPressEvent(QKeyEvent *event);
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    Hero* hero;
    void initView();
    void initGame();
    int width;
    int height;
    int margin;
};

#endif // SCENE_H
