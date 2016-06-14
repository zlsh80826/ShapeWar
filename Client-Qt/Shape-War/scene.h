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
#include <QtWebSockets/QWebSocket>

class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QWidget* parent = 0, const QUrl &url = QUrl());
    Hero* self;

    // test object
    Triangle* testTriangle;
    Rectangle* testRectangle;
    Pentagon* testPentagon;
    Bullet* testBullet;

    // connection
    QWebSocket m_webSocket;
    QUrl m_url;

public slots:
    void startGame();
    void gameOver();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onConnected();
    void onTextMessageReceived(QString message);

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
