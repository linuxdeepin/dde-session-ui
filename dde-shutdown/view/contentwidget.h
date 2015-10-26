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

private:
    void initUI();
    void initConnect();
    void enterKeyPushed();

    RoundItemButton *m_currentSelectedBtn = nullptr;
    QList<RoundItemButton *> *m_btnsList;
};
#endif // CONTENTVIEWWIDGET
