#ifndef BOARDCASTBOARD_H
#define BOARDCASTBOARD_H
#include <QTextEdit>

class BoardcastBoard : public QTextEdit {
    Q_OBJECT
public:
    BoardcastBoard(QWidget *parent = 0);

protected:
    void focusOutEvent(QFocusEvent *event);
    void focusInEvent(QFocusEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:

};

#endif // BOARDCASTBOARD_H
