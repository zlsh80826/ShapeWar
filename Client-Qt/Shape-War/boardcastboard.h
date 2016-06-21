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
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;

    // override mouse event to ignore events passing to this widget
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    const int boardcastHeight = 120;
};

#endif // BOARDCASTBOARD_H
