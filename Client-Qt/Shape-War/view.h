#ifndef VIEW_H
#define VIEW_H

#include "hero.h"
#include "scene.h"
#include <QDebug>
#include <QGraphicsView>
#include <QJsonDocument>
#include <QJsonObject>
#include <QKeyEvent>
#include <QTimer>
#include <QWebSocket>
#include <pentagon.h>
#include <rectangle.h>
#include <triangle.h>

#include <QLabel>
#include <QPair>
#include <QPushButton>
#include <QButtonGroup>

class View : public QGraphicsView {
    Q_OBJECT

public:
    View(Scene *scene, QWebSocket &ws);

public slots:
    void onSelfPosChanged();
    void sendControlToServer();
    void showUpgrateOptions();
    void onUpgradePointChanged();
    void onPropertyBtnClicked(int);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;


private:
    const int viewWidth = 960;
    const int viewHeight = 768;
    const int maxViewWidth = 1920;
    const int maxViewHeight = 1080;
    qreal calcRargetAngle(QPointF &mouseP);
    Self *self;

    bool key_w_pressed;
    bool key_a_pressed;
    bool key_s_pressed;
    bool key_d_pressed;

    bool mouseClicked;
    qreal targetAngle;

    QTimer *sendDelayTimer;
    const int sendDelay = 500;

    // for upgrade option
    const int buttonLen = 30;
    const int buttonDistance = 33;
    const int labelWidth = 150;
    QPushButton *expandBtn;
    bool isExpanded;
    QVector<QPair<QLabel *, QPushButton *> *> properties;
    QButtonGroup *propertyBtnPtrGroup;
    QWebSocket &ws;

    const int InfoHeightOffset = 120;
    int InfoCenterX = viewWidth/2;
    int InfoCenterY = viewHeight - InfoHeightOffset;

    QTimer *sec;
    int sends;
private slots:
    void print_freq();
};

#endif // VIEW_H
