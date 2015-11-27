#ifndef LOCKFRAME
#define LOCKFRAME

#include <QKeyEvent>

#include <QDBusConnection>
#include <QDBusAbstractAdaptor>

#include "backgroundlabel.h"
#include "lockmanager.h"
#include "boxframe.h"

const QString DBUS_PATH = "/com/deepin/dde/lockFront";
const QString DBUS_NAME = "com.deepin.dde.lockFront";


class LockFrontDBus;
class LockFrame: public BoxFrame
{
    Q_OBJECT
public:
    LockFrame(QString url, QWidget* parent=0);
    ~LockFrame();
public slots:
    void updateScreenPosition(QRect rect);
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    LockManager* m_lockManager;
};

class LockFrontDBus : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.lockFront")

public:
    LockFrontDBus(LockFrame* parent);
    ~LockFrontDBus();
    Q_SLOT qulonglong lockResult();
private:
    LockFrame* m_parent;
};

#endif // LOCKFRAME

