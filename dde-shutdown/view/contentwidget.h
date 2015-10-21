#ifndef CONTENTVIEWWIDGET
#define CONTENTVIEWWIDGET
#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include "rounditembutton.h"
#include "util_signalmanager.h"
#include "util_updateui.h"

class ShutDownFrame: public QFrame
{
    Q_OBJECT
public:
    ShutDownFrame(QWidget* parent=0);
    ~ShutDownFrame();

    RoundItemButton* m_shutdownButton;
    RoundItemButton* m_restartButton;
    RoundItemButton* m_suspendButton;
    RoundItemButton* m_lockButton;
    RoundItemButton* m_logoutButton;
//    RoundItemButton* m_userSwitchButton;
    QHBoxLayout* m_buttonLayout;
    QVBoxLayout* m_Layout;
signals:
    void keyLeft();
    void keyRight();
    void pressEnterAction();
    void ShutDownFrameActions(QString buttonId);

public slots:
    void setPreviousChildFocus();
    void setNextChildFocus();
//    void setUserSwitchButton(bool showing);

    void ShutDownAction();
    void setButtonGroupMutex(QString buttonId);
    void updateStyleSheet();
    void test(QString test);
private:
    void initUI();
    void initConnect();

    int m_count = 0;
    bool m_previousChanged = false;
    bool m_nextChanged = false;
};
#endif // CONTENTVIEWWIDGET
