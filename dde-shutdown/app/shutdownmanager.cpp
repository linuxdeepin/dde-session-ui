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

    connect(m_content->m_shutdownFrame, &ShutDownFrame::ShutDownFrameActions, this, &ShutdownManager::powerAction);
}
void ShutdownManager::initUI() {

    setObjectName("ShutdownManager");
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    resize(qApp->desktop()->screenGeometry().size());

    m_backgroundLabel = new BackgroundLabel(true, this);
    m_content = new MainFrame(this);

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
    m_sessionInterface = new DBusSessionManagerInterface("com.deepin.SessionManager", "/com/deepin/SessionManager",
                                                         QDBusConnection::sessionBus(), this);
}

void ShutdownManager::switchToGreeter()
{
    QProcess *process = new QProcess;
    connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), process, &QProcess::deleteLater);
    process->start("dde-switchtogreeter");
    qApp->quit();
}

void ShutdownManager::powerAction(const ShutDownFrame::Actions action)
{
    switch (action)
    {
    case ShutDownFrame::Shutdown:       m_sessionInterface->RequestShutdown();        break;
    case ShutDownFrame::Restart:        m_sessionInterface->RequestReboot();          break;
    case ShutDownFrame::Suspend:        m_sessionInterface->RequestSuspend();       break;
    case ShutDownFrame::Lock:           m_sessionInterface->RequestLock();          break;
    case ShutDownFrame::Logout:         m_sessionInterface->RequestLogout();          break;
    case ShutDownFrame::SwitchUser:     switchToGreeter();                          break;
    default:                            qWarning() << "action: " << action << " not handled";
    }

    qApp->quit();
}

void ShutdownManager::hideBtns(const QStringList &btnsName)
{
    m_content->hideBtns(btnsName);
}

void ShutdownManager::disableBtns(const QStringList &btnsName)
{
    m_content->disableBtns(btnsName);
}

void ShutdownManager::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
    case Qt::Key_Escape:        qApp->quit();                   break; // must break;
    case Qt::Key_Return:        /* same as enter */
    case Qt::Key_Enter:         emit pressEnter();              break;
    case Qt::Key_Left:          emit DirectKeyLeft();           break;
    case Qt::Key_Right:         emit DirectKeyRight();          break;
    default:;
    }
}
void ShutdownManager::mouseReleaseEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton) {
        qApp->quit();
    }
}

ShutdownManager::~ShutdownManager()
{
}
