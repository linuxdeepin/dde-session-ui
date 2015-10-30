#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>

#include "shutdownmanager.h"

ShutdownManager::ShutdownManager(QWidget* parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
    initData();
}

void ShutdownManager::initConnect() {
    connect(this, SIGNAL(DirectKeyLeft()) , m_content, SIGNAL(OutKeyLeft()));
    connect(this, SIGNAL(DirectKeyRight()), m_content, SIGNAL(OutKeyRight()));
    connect(this, SIGNAL(pressEnter()), m_content, SIGNAL(pressEnterAction()));

    connect(m_content->m_shutdownFrame, SIGNAL(ShutDownFrameActions(QString)), this, SLOT(powerAction(QString)));
}
void ShutdownManager::initUI() {

    setObjectName("ShutdownManager");
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    resize(qApp->desktop()->screenGeometry().size());

    m_backgroundLabel = new BackgroundLabel(true, this);
    m_content = new MainFrame(m_mode);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addWidget(m_content);
    m_Layout->addStretch();
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

    showFullScreen();
    activateWindow();

}

void ShutdownManager::initData() {
    m_sessionInterface = new SessionManageInterfaceManagement(this);
}
void ShutdownManager::powerAction(QString action) {
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
void ShutdownManager::keyPressEvent(QKeyEvent* e) {

    if (e->key()==Qt::Key_Escape) {
        qApp->quit();
    }
    if (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter) {
        emit pressEnter();
    }
    if (e->key()==Qt::Key_Left) {
        emit  DirectKeyLeft();
    } else if (e->key()==Qt::Key_Right) {
        emit DirectKeyRight();
    }

}
void ShutdownManager::mouseReleaseEvent(QMouseEvent* e) {
    if (e->button() == Qt::LeftButton) {
        qApp->quit();
    }
}

ShutdownManager::~ShutdownManager()
{
}
