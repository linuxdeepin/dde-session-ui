#ifndef SHUTDOWNFRAME
#define SHUTDOWNFRAME

#include <QFrame>

#include "boxframe.h"
#include "shutdownmanager.h"

class ShutdownFrame:public BoxFrame
{
    Q_OBJECT
public:
    ShutdownFrame(QWidget* parent = 0);
    ~ShutdownFrame();
public slots:
    void updateScreenPosition();
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    ShutdownManager* m_shutdownManager;
};

#endif // SHUTDOWNFRAME

