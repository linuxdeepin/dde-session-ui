#ifndef LOCKFRAME_H
#define LOCKFRAME_H

#include <QFrame>

#include "userwidget.h"
#include "controlwidget.h"
#include "passwdedit.h"
#include "dbus/dbuslockservice.h"

class LockFrame : public QFrame
{
    Q_OBJECT

public:
    LockFrame();

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private:
    void unlock();

private:
    UserWidget *m_userWidget;
    ControlWidget *m_controlWidget;
    PassWdEdit *m_passwordEdit;
    DBusLockService *m_lockInter;
};

#endif // LOCKFRAME_H
