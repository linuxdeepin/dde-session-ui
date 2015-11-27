#ifndef BOXFRAME_H
#define BOXFRAME_H

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QEvent>

#include <QFrame>
#include <QLabel>

class BoxFrame : public QFrame
{
    Q_OBJECT

public:
    explicit BoxFrame(QWidget* parent = 0);
    explicit BoxFrame(const QString url, QWidget *parent = 0);
    ~BoxFrame();
signals:
    void screenChanged(QRect rect);
protected:
    void resizeEvent(QResizeEvent *e);
    void keyPressEvent(QKeyEvent *e);
};

#endif // BOXFRAME_H
