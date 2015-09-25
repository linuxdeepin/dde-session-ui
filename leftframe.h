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
    void setViewMode(int mode);//0 is the login mode; 1 is the locking screen mode;
                               //2 is the shut-down mode;
private:
    QVBoxLayout* m_Layout;

    void setBottomWidget(QWidget * bottomWidget);

    LogoWidget* m_LogoWidget;
    TimeWidget* m_TimeWidget;
};

#endif // LeftSideView
