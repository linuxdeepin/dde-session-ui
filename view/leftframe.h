#ifndef LeftSideView
#define LeftSideView

#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QVBoxLayout>
#include "leftcornerwidget.h"
class LeftFrame: public QFrame
{
    Q_OBJECT
public:
    LeftFrame(int mode, QWidget *parent=0);
    ~LeftFrame();
public slots:
    void setMode(int mode);//0 is the login mode; 1 is the locking screen mode;
                       //2 is the shut-down mode; 3 is the quick shutdown mode;
private:
    int m_mode;
    QVBoxLayout* m_Layout;
    LogoWidget* m_LogoWidget;
    TimeWidget* m_TimeWidget;
};

#endif // LeftSideView
