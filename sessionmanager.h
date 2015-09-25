#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H
#include <QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>

#include "leftframe.h"
#include "mainframe.h"
#include "powermenuframe.h"

class SessionManager : public QFrame
{
    Q_OBJECT

public:
    SessionManager(QWidget *parent = 0);
    ~SessionManager();
    int openmode = 0;//0 is the login mode; 1 is the locking screen mode; 2 is the shut-down mode;
public slots:
    void setBackgroundImage(QString imageUrl);
    void keyPressEvent(QKeyEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void setMode(int mode);
signals:
    void DirectKeyLeft();
    void DirectKeyRight();
private:
    int m_mode=2;
    QHBoxLayout* m_Layout;

    LeftFrame* m_leftContent;
    MainFrame* m_content;
    PowerMenuFrame* m_rightContent;
};

#endif // SESSIONMANAGER_H
