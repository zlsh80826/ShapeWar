#ifndef BOARDCASTBOARD_H
#define BOARDCASTBOARD_H
#include <QTextEdit>
#include <QMouseEvent>

class BoardcastBoard : public QTextEdit {
    Q_OBJECT
public:
    // constructor
    BoardcastBoard(QWidget *parent = 0);

protected:
    // focus event for debug
    void focusOutEvent(QFocusEvent *event);
    void focusInEvent(QFocusEvent *event);

    // override mouse event to ignore events passing to this widget
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    const int boardcastHeight = 120;
};

#endif // BOARDCASTBOARD_H
