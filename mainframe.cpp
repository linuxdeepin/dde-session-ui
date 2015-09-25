#include "mainframe.h"
#include <QtCore/QObject>

MainFrame::MainFrame(int mode, QWidget *parent)
    : QFrame(parent)
{
    qDebug() << "MainFrame Start!";
    xmode = mode;
    m_shutdownFrame = new ShutDownFrame;
    qDebug() << "MainFrame shutdown";
    m_loginFrame = new LoginFrame(true);
    qDebug() << "MainFrame LoginFrame";
    m_quickShutDownFrame = new QuickShutDownFrame;
    qDebug() << "MainFrame quickShutDown";
    m_contentWidget = new QStackedWidget;  
    m_contentWidget->addWidget(m_loginFrame);
    m_contentWidget->addWidget(m_quickShutDownFrame);
    m_contentWidget->addWidget(m_shutdownFrame);
    m_contentWidget->setCurrentWidget(m_shutdownFrame);
    setViewMode(mode);
    m_contentLayout = new QVBoxLayout;
    m_contentLayout->setMargin(0);
    m_contentLayout->setSpacing(0);
    m_contentLayout->addStretch();
    m_contentLayout->addWidget(m_contentWidget);
    m_contentLayout->addStretch();



    m_Layout = new QHBoxLayout;
    m_contentLayout->setMargin(0);
    m_contentLayout->setSpacing(0);
    m_Layout->addLayout(m_contentLayout);

    qDebug() << "MainFrame child:";
    setLayout(m_Layout);

    connect(this, SIGNAL(OutKeyLeft()), this ,SLOT(DirectLeft()));
    connect(this, SIGNAL(OutKeyRight()), this, SLOT(DirectRight()));


}
MainFrame::~MainFrame()
{}
void MainFrame::switchMode(bool isSwitch) {

    if (!isSwitch) {
        setViewMode(1);
    }
}
void MainFrame::setViewMode(int mode) {
    if (mode==0) {
        xmode = 0;
        m_contentWidget->setCurrentIndex(0);
    } else if (mode==1) {
        xmode = 1;
        m_contentWidget->setCurrentIndex(1);
    } else {
        xmode = 2;
        m_contentWidget->setCurrentIndex(2);

    }
}
void MainFrame::changeView(bool a) {
    qDebug() << "clicked" << a;
}
void MainFrame::DirectLeft() {
   if (xmode == 0) {}
   else if (xmode==1) {
        emit m_quickShutDownFrame->keyLeft();
   } else {
       emit m_shutdownFrame->keyLeft();
   }
}
void MainFrame::DirectRight() {
    qDebug() << "MainFrame: xmode:" << xmode;
    if (xmode ==0 ) {}
    else if (xmode == 1) {
        emit m_quickShutDownFrame->keyRight();
    } else {
        emit m_shutdownFrame->keyRight();
    }
}
