#include <QtCore/QObject>
#include "sessionmanager.h"
#include <QApplication>
#include <QtCore/QFile>
SessionManager::SessionManager(QWidget *parent)
    : QFrame(parent)
{
    qDebug() << "build SessionManager start!";
     setFixedSize(1440, 900);
     setObjectName("SessionManager");
     setWindowFlags(Qt::FramelessWindowHint);


     m_leftContent = new LeftFrame(m_mode);
     qDebug() << "build leftConent";
     m_content = new MainFrame(m_mode);
     qDebug() << "build middleContent";
     m_rightContent = new PowerMenuFrame(m_mode);
     qDebug() << "build PowerMenuContent";
     m_Layout = new QHBoxLayout;
     m_Layout->setMargin(0);
     m_Layout->setSpacing(0);
     m_Layout->addWidget(m_leftContent);
     m_Layout->addWidget(m_content);


     QFile qssFile(":/skin/skin.qss");
     QString qss;
     qssFile.open(QFile::ReadOnly);
     if(qssFile.isOpen())
     {
         qss = QLatin1String(qssFile.readAll());
         this->setStyleSheet(qss);
         qssFile.close();
     }
     setFocusPolicy(Qt::StrongFocus);
     setLayout(m_Layout);

    connect(this, SIGNAL(DirectKeyLeft()) , m_content, SIGNAL(OutKeyLeft()));
    connect(this, SIGNAL(DirectKeyRight()), m_content, SIGNAL(OutKeyRight()));
    qDebug() << "build SessionManager finished!";
}

SessionManager::~SessionManager()
{
}
void SessionManager::setMode(int mode) {
    m_mode = mode;
}
void SessionManager::setBackgroundImage(QString imageUrl) {
    QString backgroundstyle("QFrame#SessionManager { background-color: transparent;"
                            "background-image:url(%1);}");
    backgroundstyle = backgroundstyle.arg(imageUrl);
    setStyleSheet(backgroundstyle);
}
void SessionManager::keyPressEvent(QKeyEvent *e) {
    if (e->key()==Qt::Key_Escape) {
        qApp->quit();
    }

    if (e->key()==Qt::Key_Left) {
        emit  DirectKeyLeft();
    } else if (e->key()==Qt::Key_Right) {
        emit DirectKeyRight();
    }
    switch (e->key()) {
    case Qt::Key_F1:
        m_mode = 0;
        m_leftContent->setViewMode(m_mode);
        m_content->setViewMode(m_mode);
        break;
    case Qt::Key_F2:
        m_mode = 1;
        m_leftContent->setViewMode(m_mode);
        m_content->setViewMode(m_mode);
        break;
    case Qt::Key_F5:
        m_mode = 1;
        break;
    case Qt::Key_F3:
        m_mode = 2;
        m_leftContent->setViewMode(m_mode);
        m_content->setViewMode(m_mode);
        break;
    }
}
void SessionManager::mouseReleaseEvent(QMouseEvent *e) {
    qDebug() << "leftButton" << m_mode;
    if (e->button() == Qt::LeftButton) {
        if (m_mode==1) {
            m_mode = 0;
            qDebug() << "changeNew mode:" << m_mode;
            m_leftContent->setViewMode(m_mode);
            m_content->setViewMode(m_mode);
        }
    }
}
