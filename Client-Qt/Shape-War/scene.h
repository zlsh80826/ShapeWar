#ifndef SCENE_H
#define SCENE_H

#include "polygongroup.h"
#include <QColor>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QVector>
#include <QtWebSockets/QWebSocket>
#include <bullet.h>
#include <hero.h>
#include <math.h>
#include <polygon.h>
#include <self.h>

class Scene : public QGraphicsScene {
    Q_OBJECT
public:
    Scene(QWidget *parent = 0);
    Self *self;

    PolygonGroup<Polygon> *triangles;
    PolygonGroup<Polygon> *rectangles;
    PolygonGroup<Polygon> *pentagons;
    PolygonGroup<Bullet> *bullets;

    // connection
    QWebSocket m_webSocket;
    QUrl *dummy_url;

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
    void fillServerInfo(QString &serverUrl, QString &port);

    QTimer *sec;
    int recvs;
private slots:
    void print_freq();
};

#endif // SCENE_H
