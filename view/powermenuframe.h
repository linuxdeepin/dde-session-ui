#ifndef RIGHTFRAME
#define RIGHTFRAME

#include <QtWidgets/QFrame>
#include <QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
class PowerMenuFrame: public QFrame
{
    Q_OBJECT
public:
    PowerMenuFrame(int mode, QWidget*parent=0);
    ~PowerMenuFrame();
public:
    void setMode(int mode);
private:
    int m_mode;
    QHBoxLayout* m_ContentLayout;
    QVBoxLayout* m_Layout;
//    QPushButton* m_userSwitchButton;
//    QPushButton* m_powerMenuButton;
};
#endif // RIGHTFRAME

