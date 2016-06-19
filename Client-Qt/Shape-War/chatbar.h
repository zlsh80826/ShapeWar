#ifndef CHATBAR_H
#define CHATBAR_H
#include <QJsonDocument>
#include <QJsonObject>
#include <QLineEdit>
#include <QString>
#include <QTimer>
#include <QWebSocket>

class ChatBar : public QLineEdit {
    Q_OBJECT
public:
    ChatBar(QString partUrl, QWidget *parent = 0);
    void startChat();
    void sendTextToServer();
    void setParentWidth(int width);

protected:
    void focusOutEvent(QFocusEvent *event);
    void focusInEvent(QFocusEvent *event);

private:
    QUrl *chat_url;
    QWebSocket chat_webSocket;
    QTimer *upTimer;
    QTimer *downTimer;
    int parentWidth;
    int posY;
    const int chatBarHeight = 30;
    const int maxPosY = 0;
    const int minPosY = -40;

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);
    void down();
    void up();
};

#endif // CHATBAR_H
