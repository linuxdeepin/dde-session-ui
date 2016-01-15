#ifndef SHUTDOWNFRAME
#define SHUTDOWNFRAME

#include <QFrame>
#include "boxframe.h"
#include "shutdownmanager.h"

const QString DBUS_PATH = "/com/deepin/dde/shutdownFront";
const QString DBUS_NAME = "com.deepin.dde.shutdownFront";

class ShutdownFrontDBus;
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

class ShutdownFrontDBus : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.shutdownFront")

public:
    ShutdownFrontDBus(ShutdownFrame* parent);
    ~ShutdownFrontDBus();
    Q_SLOT qulonglong result();
private:
    ShutdownFrame* m_parent;
};
#endif // SHUTDOWNFRAME

