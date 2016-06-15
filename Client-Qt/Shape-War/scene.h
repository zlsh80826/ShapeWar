#ifndef SCENE_H
#define SCENE_H

#include <QColor>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QVector>
#include <QtWebSockets/QWebSocket>
#include <bullet.h>
#include <hero.h>
#include <math.h>
#include <pentagon.h>
#include <self.h>
#include "rectangle.h"
#include "triangle.h"
#include "polygongroup.h"

class Scene : public QGraphicsScene {
    Q_OBJECT
public:
    Scene(QWidget *parent = 0, const QUrl &url = QUrl());
    Self *self;

    // test object
    PolygonGroup<Triangle> *triangles;
    PolygonGroup<Rectangle> *rectangles;
    PolygonGroup<Pentagon> *pentagons;
    Bullet *testBullet;

    // connection
    QWebSocket m_webSocket;
    QUrl m_url;

public slots:
    void startGame();
    void gameOver();
    void slotAcceptUserLogin(QString &, QString &, QString &, QString &, bool);

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
    QVector<Hero *> heroes;
};

#endif // SCENE_H
