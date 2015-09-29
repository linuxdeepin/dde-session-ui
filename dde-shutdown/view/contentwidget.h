#ifndef CONTENTVIEWWIDGET
#define CONTENTVIEWWIDGET
#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include "widgets/sessionbutton.h"
#include "widgets/passwdedit.h"
#include "user/userframe.h"
#include "signalmanagement/signalmanager.h"

class ShutDownFrame: public QFrame
{
    Q_OBJECT
public:
    ShutDownFrame(QWidget *parent=0);
    ~ShutDownFrame();
signals:
    void keyLeft();
    void keyRight();
    void pressEnterAction();
    void ShutDownFrameActions(QString buttonId);

public slots:
    void setPreviousChildFocus();
    void setNextChildFocus();
//    void setUserSwitchButton(bool showing);
    void initConnect();
    void ShutDownAction();
    void setButtonGroupMutex(QString buttonId);

public:
    SessionButton* m_shutdownButton;
    SessionButton* m_restartButton;
    SessionButton* m_suspendButton;
    SessionButton* m_lockButton;
    SessionButton* m_logoutButton;
//    SessionButton* m_userSwitchButton;
    QHBoxLayout* m_buttonLayout;
    QVBoxLayout* m_Layout;
private:
    int m_count = 0;
    bool m_previousChanged = false;
    bool m_nextChanged = false;
};
class LoginFrame: public QFrame
{
    Q_OBJECT
public:
    LoginFrame(QWidget *parent=0);
    ~LoginFrame();
    void setMode(int mode);
signals:
    void keyLeft();
    void keyRight();
private:
    UserFrame* m_userFrame;
    PassWdEdit* m_passwordEdit;
    QHBoxLayout* m_passwdLineEditLayout;

    QVBoxLayout* m_Layout;
};
class QuickShutDownFrame: public QFrame
{
    Q_OBJECT
public:
    QuickShutDownFrame(QWidget *parent=0);
    ~QuickShutDownFrame();
signals:
    void keyLeft();
    void keyRight();
    void pressEnterAction();
public slots:
    void setPreviousChildFocus();
    void setNextChildFocus();
public:
    SessionButton* m_quickShutDownButton;
    SessionButton* m_quickRestartButton;
    SessionButton* m_quickSuspendButton;
    QHBoxLayout* m_Layout;
};
#endif // CONTENTVIEWWIDGET
