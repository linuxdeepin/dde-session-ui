#ifndef LOCKFRAME_H
#define LOCKFRAME_H

#include <QFrame>

class LockFrame : public QFrame
{
    Q_OBJECT

public:
    LockFrame();

private:
    void keyPressEvent(QKeyEvent *e);
};

#endif // LOCKFRAME_H
