#ifndef LOCKFRAME_H
#define LOCKFRAME_H

#include <QFrame>

#include "sessionwidget.h"
#include "userwidget.h"
#include "controlwidget.h"
#include "passwdedit.h"

class LockFrame : public QFrame
{
    Q_OBJECT

public:
    LockFrame();

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private:
    void unlock();
    void chooseSession(const QString &sessionName);

private:
    SessionWidget *m_sessionWidget;
    UserWidget *m_userWidget;
    ControlWidget *m_controlWidget;
    PassWdEdit *m_passwordEdit;
};

#endif // LOCKFRAME_H
