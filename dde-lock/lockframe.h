#ifndef LOCKFRAME
#define LOCKFRAME

#include <QKeyEvent>
#include "lockmanager.h"
#include "boxframe.h"
class LockFrame: public BoxFrame
{
    Q_OBJECT
public:
    LockFrame(QWidget* parent=0);
    ~LockFrame();
public slots:
    void updateScreenPosition(QRect rect);
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    LockManager* m_lockManager;
};

#endif // LOCKFRAME

