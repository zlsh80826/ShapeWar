#ifndef CHATBAR_H
#define CHATBAR_H
#include <QLineEdit>

class ChatBar : public QLineEdit {
public:
    ChatBar(QWidget * parent = 0);
    void startChat();
    void sendTextToServer();

protected:
    void focusOutEvent(QFocusEvent * event);
    void focusInEvent(QFocusEvent * event);

};

#endif // CHATBAR_H
