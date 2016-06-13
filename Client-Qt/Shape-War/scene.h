#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QColor>
#include <hero.h>
#include <triangle.h>
#include <rectangle.h>
#include <pentagon.h>
#include <bullet.h>
#include <QKeyEvent>
#include <math.h>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QWidget* parent = 0);
    Hero* self;

    // test object
    Triangle* testTriangle;
    Rectangle* testRectangle;
    Pentagon* testPentagon;
    Bullet* testBullet;

public slots:
    void startGame();
    void gameOver();

protected:
    void drawBackground(QPainter *painter, const QRectF &rect);

private:
    void initGame();
    int width;
    int height;
    int margin;
    QColor backgroundColor;

};

#endif // SCENE_H
