#ifndef ContentFrame
#define ContentFrame
#include <QtWidgets/QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtCore>
#include "contentwidget.h"

class MainFrame : public QFrame
{
    Q_OBJECT
public:
    MainFrame(int mode, QWidget* parent=0);
    ~MainFrame();

public slots:
    void setMode(int mode);//0 is the login mode; 1 is the locking screen mode;
                               //2 is the shut-down mode; 3 is the quick shut-down mode;
    void changeView(bool a);
    void DirectLeft();
    void DirectRight();
    void EnterAction();
signals:
    void OutKeyLeft();
    void OutKeyRight();
    void pressEnterAction();
private:
    int m_mode;
public:
    ShutDownFrame* m_shutdownFrame;
    LoginFrame* m_loginFrame;
    QuickShutDownFrame* m_quickShutDownFrame;
    QStackedWidget* m_contentWidget;
    QHBoxLayout* m_Layout;
    QVBoxLayout* m_contentLayout;
    QVBoxLayout* m_contentRightLayout;

};

#endif // ContentFrame
