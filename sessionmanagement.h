#ifndef SessionManagerTool_H
#define SessionManagerTool_H
#include <QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>

#include "leftframe.h"
#include "mainframe.h"
#include "powermenuframe.h"
#include "dbusmanagement.h"
#include "background/backgroundlabel.h"
#include "signalmanagement/signalmanager.h"

class SessionManagement : public QFrame
{
    Q_OBJECT

public:
    SessionManagement(QWidget *parent = 0);
    ~SessionManagement();


public slots:
    void setBackgroundImage(QString imageUrl);
    void keyPressEvent(QKeyEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void setMode(int mode);
    void powerAction(QString action);
    void initConnect();
    void testing();
signals:
    void DirectKeyLeft();
    void DirectKeyRight();
    void pressEnter();
private:
    int m_mode=2;

    QHBoxLayout* m_Layout;
    LeftFrame* m_leftContent;
    MainFrame* m_content;
    PowerMenuFrame* m_rightContent;
    SessionManageInterfaceManagement* m_sessionInterface;
    BackgroundLabel* m_backgroundLabel;
};

#endif // SessionManagerTool_H
