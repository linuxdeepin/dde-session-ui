#ifndef CONTENTVIEWWIDGET
#define CONTENTVIEWWIDGET
#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>


#include "sessionbutton.h"
#include "passwdedit.h"

class ShutDownFrame: public QFrame
{
    Q_OBJECT
public:
    ShutDownFrame(QWidget *parent=0);
    ~ShutDownFrame();
signals:
    void keyLeft();
    void keyRight();
public slots:
//    void keyPressEvent(QKeyEvent *e);
public slots:
    void setPreviousChildFocus();
    void setNextChildFocus();
    void setUserSwitchButton(bool showing);
public:
    SessionButton* m_shutdownButton;
    SessionButton* m_restartButton;
    SessionButton* m_suspendButton;
    SessionButton* m_lockButton;
    SessionButton* m_logoutButton;
    SessionButton* m_userSwitchButton;
    QHBoxLayout* m_buttonLayout;
    QVBoxLayout* m_Layout;
};
class LoginFrame: public QFrame
{
    Q_OBJECT
public:
    LoginFrame(bool login, QWidget *parent=0);
    ~LoginFrame();
    void setLogin(bool isLogin);
private:
    bool m_login = true;
    QLabel* m_userImageLabel;
    QLabel* m_userNameLabel;
    PassWdEdit* m_passwordEdit;
    QHBoxLayout* m_userImageLayout;
    QHBoxLayout* m_userNameLayout;
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

