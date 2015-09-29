#include "mainframe.h"
#include <QtCore/QObject>

MainFrame::MainFrame(int mode, QWidget *parent)
    : QFrame(parent)
{
    m_mode = mode;
    m_shutdownFrame = new ShutDownFrame;
    m_loginFrame = new LoginFrame;
    m_quickShutDownFrame = new QuickShutDownFrame;

    m_contentWidget = new QStackedWidget;  
    m_contentWidget->addWidget(m_loginFrame);
    m_contentWidget->addWidget(m_shutdownFrame);
    m_contentWidget->addWidget(m_quickShutDownFrame);
    m_contentWidget->setCurrentWidget(m_shutdownFrame);
    setMode(m_mode);

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

    setLayout(m_Layout);

    connect(this, SIGNAL(OutKeyLeft()), this ,SLOT(DirectLeft()));
    connect(this, SIGNAL(OutKeyRight()), this, SLOT(DirectRight()));
    connect(this, SIGNAL(pressEnterAction()), this, SLOT(EnterAction()));
}
MainFrame::~MainFrame()
{}
void MainFrame::setMode(int mode) {
    m_mode = mode;
    if (m_mode==0) {
        m_contentWidget->setCurrentIndex(0);
        m_loginFrame->setMode(m_mode);
    } else if (m_mode==1) {
        m_contentWidget->setCurrentIndex(0);
        m_loginFrame->setMode(m_mode);
    } else if (m_mode==2){
        m_contentWidget->setCurrentIndex(1);
    } else {
        m_contentWidget->setCurrentIndex(2);
    }
}
void MainFrame::changeView(bool a) {
    qDebug() << "clicked" << a;
}
void MainFrame::DirectLeft() {
   if (m_mode == 0) {
       emit m_loginFrame->keyLeft();
   } else if (m_mode == 1) {
        emit m_loginFrame->keyLeft();
   } else if (m_mode == 2){
       emit m_shutdownFrame->keyLeft();
   } else {
       emit m_quickShutDownFrame->keyLeft();
   }
}
void MainFrame::DirectRight() {
    if (m_mode ==0 ) {
        emit m_loginFrame->keyRight();
    } else if (m_mode == 1) {
        emit m_loginFrame->keyRight();
    } else if (m_mode == 2){
        emit m_shutdownFrame->keyRight();
    } else {
        emit m_quickShutDownFrame->keyRight();
    }
}
void MainFrame::EnterAction() { 
    if (m_mode == 2) {  
        emit m_shutdownFrame->pressEnterAction();
    }
}
