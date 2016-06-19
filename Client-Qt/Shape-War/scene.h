#ifndef SCENE_H
#define SCENE_H

#include "polygongroup.h"
#include <QColor>
#include <QGraphicsView>
#include <QHash>
#include <QKeyEvent>
#include <QSet>
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

    // connection
    QWebSocket m_webSocket;
    QUrl *dummy_url;

    QString getPartUrl() const;

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
    int self_id = -1;
    int width;
    int height;
    int margin;
    QColor backgroundColor;
    QHash<int, Hero *> heroes;
    void fillServerInfo();
    QString partUrl;
    int cleanTimer;
    QSet<int> cleanHit;

    QTimer *sec;
    int recvs;
private slots:
    void print_freq();
};

#endif // SCENE_H
