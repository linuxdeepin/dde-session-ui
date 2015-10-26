#ifndef SWITCHFRAME
#define SWITCHFRAME
#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

class SwitchFrame:public QFrame
{
    Q_OBJECT
public:
    SwitchFrame(QWidget* parent=0);
    ~SwitchFrame();

signals:
    void triggerSwitchSession();
    void triggerSwitchUser();
    void triggerPower();

private:
    void initUI();
    void initConnect();

    QPushButton* m_switchSessionButton;
    QPushButton* m_switchUserButton;
    QPushButton* m_powerButton;
    QHBoxLayout* m_Layout;

};
#endif // SWITCHFRAME

