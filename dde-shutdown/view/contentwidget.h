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

    enum Actions {
        Shutdown,
        Restart,
        Suspend,
        Lock,
        Logout,
        SwitchUser,
    };

signals:
    void keyLeft();
    void keyRight();
    void pressEnterAction();
    void ShutDownFrameActions(const Actions action);

public slots:
    void setPreviousChildFocus();
    void setNextChildFocus();
    void showTips(const QString &tips);
    void hideBtns(const QStringList &btnsName);
    void disableBtns(const QStringList &btnsName);

private:
    void initUI();
    void initConnect();
    void enterKeyPushed();
    void hideBtn(const QString &btnName);
    void disableBtn(const QString &btnName);

    RoundItemButton *m_currentSelectedBtn = nullptr;
    RoundItemButton *m_shutdownButton;
    RoundItemButton *m_restartButton;
    RoundItemButton *m_suspendButton;
    RoundItemButton *m_lockButton;
    RoundItemButton *m_logoutButton;
    RoundItemButton *m_switchUserBtn;
    QList<RoundItemButton *> *m_btnsList;

    QWidget *m_tipsWidget;
    QLabel *m_tipsLabel;
};
#endif // CONTENTVIEWWIDGET
