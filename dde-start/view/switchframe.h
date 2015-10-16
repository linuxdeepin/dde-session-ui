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
    SwitchFrame(QWidget*parent=0);
    ~SwitchFrame();
public slots:
    void initConnect();
    void testing(QString switchId);
    void switchUser();
    void switchDesktopEnvironment();
    void switchPowerMenu();
signals:
    void switchItem(QString switchItem);
private:
    QPushButton* m_switchSystemButton;
    QPushButton* m_switchUserButton;
    QPushButton* m_powerButton;
    QHBoxLayout* m_Layout;

};
#endif // SWITCHFRAME

