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

    ShutDownFrame* m_shutdownFrame;
    QHBoxLayout* m_Layout;
    QVBoxLayout* m_contentLayout;
    QVBoxLayout* m_contentRightLayout;
signals:
    void OutKeyLeft();
    void OutKeyRight();
    void pressEnterAction();
public slots:
    void setMode(int mode);//0 is the login mode; 1 is the locking screen mode;
                               //2 is the shut-down mode; 3 is the quick shut-down mode;
    void changeView(bool a);
    void DirectLeft();
    void DirectRight();
    void EnterAction();

private:
    void initUI();
    void initConnect();
    int m_mode;

};

#endif // ContentFrame
