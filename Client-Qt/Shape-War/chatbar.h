#ifndef CHATBAR_H
#define CHATBAR_H
#include <QLineEdit>
#include <QString>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>

class ChatBar : public QLineEdit {
public:
    ChatBar(QString partUrl, QWidget * parent = 0);
    void startChat();
    void sendTextToServer();

protected:
    void focusOutEvent(QFocusEvent * event);
    void focusInEvent(QFocusEvent * event);

private:
    QUrl *chat_url;
    QWebSocket chat_webSocket;

private slots:
    void onConnected();
    void onTextMessageReceived(QString message);

};

#endif // CHATBAR_H
