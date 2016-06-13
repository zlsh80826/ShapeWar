#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsView>
#include <QColor>
#include <hero.h>
#include <QKeyEvent>
#include <math.h>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QWidget* parent = 0);
    Hero* self;

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


};

#endif // SCENE_H
