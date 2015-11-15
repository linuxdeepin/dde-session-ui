#ifndef SHUTDOWNWIDGET
#define SHUTDOWNWIDGET

#include <QFrame>

#include "util_updateui.h"
#include "rounditembutton.h"

class ShutdownWidget: public QFrame
{
    Q_OBJECT
public:
    ShutdownWidget(QWidget* parent = 0);
    ~ShutdownWidget();

    enum Actions {
        RequireShutdown,
        RequireRestart,
        RequireSuspend,
    };
signals:
    void shutDownWidgetAction(const Actions sd);
private:
    void initUI();
    void initConnect();

    QHBoxLayout* m_Layout;
    QList<RoundItemButton*>* m_btnList;
    RoundItemButton* m_requireShutdownButton;
    RoundItemButton* m_requireRestartButton;
    RoundItemButton* m_requireSuspendBUtton;

};
#endif // SHUTDOWNWIDGET

