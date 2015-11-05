#ifndef LOCKFRAME_H
#define LOCKFRAME_H

#include <QFrame>

#include "userwidget.h"
#include "controlwidget.h"
#include "passwdedit.h"
#include "dbus/dbuslockservice.h"
#include "dbus/dbusinputdevices.h"
#include "dbus/dbuskeyboard.h"
#include "kblayoutwidget.h"

#define LOCKSERVICE_PATH "/com/deepin/dde/lock"
#define LOCKSERVICE_NAME "com.deepin.dde.lock"

#define LOCK_KEYBOARDLAYOUT_PATH "/com/deepin/daemon/InputDevice/Keyboard"
#define LOCK_KEYBOARDLAYOUT_NAME "com.deepin.daemon.InputDevice"

class LockFrame : public QFrame
{
    Q_OBJECT

public:
    LockFrame();


    void initUI();
    void initConnect();
    void initData();
    void updateUI();
public slots:
    void setCurrentKeyboardLayout(QString keyboard_value);

protected:
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

private:
    UserWidget *m_userWidget;
    ControlWidget *m_controlWidget;
    PassWdEdit *m_passwordEdit;
    DBusLockService *m_lockInter;

    QMap<QString, QString> m_keybdInfoMap;
    DBusKeyboard* m_keyboardLayoutInterface;
    QStringList m_keybdLayoutNameList;
    QStringList keybdLayoutDescList;
    KbLayoutWidget* m_keybdLayoutWidget;

    void initBackend();
    void unlock();
};

#endif // LOCKFRAME_H
