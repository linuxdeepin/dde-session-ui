#include "mainframe.h"
#include <QtCore/QObject>

MainFrame::MainFrame(int mode, QWidget* parent)
    : QFrame(parent)
{
    m_mode = mode;
    initUI();
    initConnect();
}

void MainFrame::initConnect() {
    connect(this, SIGNAL(OutKeyLeft()), this ,SLOT(DirectLeft()));
    connect(this, SIGNAL(OutKeyRight()), this, SLOT(DirectRight()));
    connect(this, SIGNAL(pressEnterAction()), this, SLOT(EnterAction()));
}

void MainFrame::initUI() {
    m_shutdownFrame = new ShutDownFrame;
    setMode(m_mode);

    m_contentLayout = new QVBoxLayout;
    m_contentLayout->setMargin(0);
    m_contentLayout->setSpacing(0);
    m_contentLayout->addStretch();
    m_contentLayout->addWidget(m_shutdownFrame);
    m_contentLayout->addStretch();

    m_Layout = new QHBoxLayout;
    m_contentLayout->setMargin(0);
    m_contentLayout->setSpacing(0);
    m_Layout->addLayout(m_contentLayout);

    setLayout(m_Layout);
}

void MainFrame::setMode(int mode) {
    Q_UNUSED(mode);
}
void MainFrame::changeView(bool a) {
    qDebug() << "clicked" << a;
}
void MainFrame::DirectLeft() {
    emit m_shutdownFrame->keyLeft();
}
void MainFrame::DirectRight() {
    emit m_shutdownFrame->keyRight();
}
void MainFrame::EnterAction() {
    emit m_shutdownFrame->pressEnterAction();
}
MainFrame::~MainFrame()
{}
