#ifndef CONTENTVIEWWIDGET
#define CONTENTVIEWWIDGET
#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include "widgets/sessionbutton.h"
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
#endif // CONTENTVIEWWIDGET
