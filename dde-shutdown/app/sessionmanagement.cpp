#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>

#include "sessionmanagement.h"

SessionManagement::SessionManagement(QWidget *parent)
    : QFrame(parent)
{
     setObjectName("SessionManagerTool");
     setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);

     m_backgroundLabel = new BackgroundLabel(true, this);

     m_leftContent = new LeftFrame(m_mode);
     m_content = new MainFrame(m_mode);
     m_rightContent = new PowerMenuFrame(m_mode);

     m_Layout = new QHBoxLayout;
     m_Layout->setMargin(0);
     m_Layout->setSpacing(0);
     m_Layout->addWidget(m_leftContent);
     m_Layout->addWidget(m_content);
     m_Layout->addWidget(m_rightContent);
     setLayout(m_Layout);




     QFile qssFile(":/skin/main.qss");
     QString qss;
     qssFile.open(QFile::ReadOnly);
     if(qssFile.isOpen())
     {
         qss = QLatin1String(qssFile.readAll());
         this->setStyleSheet(qss);
         qssFile.close();
     }
     resize(qApp->desktop()->screenGeometry().size());
     showFullScreen();

     activateWindow();

     initConnect();

     m_sessionInterface = new SessionManageInterfaceManagement(this);
}

SessionManagement::~SessionManagement()
{
}

void SessionManagement::powerAction(QString action) {
    if (action == "ShutDownButton") {
        m_sessionInterface->ForceShutdown();
    } else if (action == "RestartButton") {
        m_sessionInterface->ForceReboot();
    } else if (action == "SuspendButton") {
        m_sessionInterface->RequestSuspend();
    } else if (action == "LockButton") {
        m_sessionInterface->RequestLock();
    } else if (action == "LogoutButton") {
        m_sessionInterface->ForceLogout();
    }
    qApp->quit();
}
void SessionManagement::setMode(int mode) {
    m_mode = mode;
}
void SessionManagement::setBackgroundImage(QString imageUrl) {
    QString backgroundstyle("QFrame#SessionManagerTool { background-color: transparent;"
                            "background-image:url(%1);}");
    backgroundstyle = backgroundstyle.arg(imageUrl);
    setStyleSheet(backgroundstyle);
}
void SessionManagement::keyPressEvent(QKeyEvent *e) {

    if (e->key()==Qt::Key_Enter) {
        emit pressEnter();
    }

    if (e->key()==Qt::Key_Escape) {
        qApp->quit();
    }
    if (e->key() == Qt::Key_Return) {
        emit pressEnter();
    }
    if (e->key()==Qt::Key_Left) {
        emit  DirectKeyLeft();
    } else if (e->key()==Qt::Key_Right) {
        emit DirectKeyRight();
    }
    switch (e->key()) {
    case Qt::Key_F6:
        m_mode = 0;
        m_leftContent->setMode(m_mode);
        m_content->setMode(m_mode);
        m_rightContent->setMode(m_mode);
        break;
    case Qt::Key_F2:
        m_mode = 1;
        m_leftContent->setMode(m_mode);
        m_content->setMode(m_mode);
        m_rightContent->setMode(m_mode);
        break;
    case Qt::Key_F3:
        m_mode = 2;
        m_leftContent->setMode(m_mode);
        m_content->setMode(m_mode);
        m_rightContent->setMode(m_mode);
        break;
    case Qt::Key_F5:
        m_mode = 3;
        m_leftContent->setMode(m_mode);
        m_content->setMode(m_mode);
        m_rightContent->setMode(m_mode);
        break;
    }

}
void SessionManagement::mouseReleaseEvent(QMouseEvent *e) {
    if (e->button() == Qt::LeftButton) {
        if (m_mode==1) {
            m_mode = 0;
            m_content->setMode(m_mode);
        }
    }
}
void SessionManagement::initConnect() {
    connect(this, SIGNAL(DirectKeyLeft()) , m_content, SIGNAL(OutKeyLeft()));
    connect(this, SIGNAL(DirectKeyRight()), m_content, SIGNAL(OutKeyRight()));
    connect(this, SIGNAL(pressEnter()), m_content, SIGNAL(pressEnterAction()));

    connect(m_content->m_shutdownFrame, SIGNAL(ShutDownFrameActions(QString)), this, SLOT(powerAction(QString)));
}
void SessionManagement::testing() {
}
