#include <QtCore/QObject>
#include <QApplication>
#include <QtCore/QFile>

#include "shutdownmanager.h"
#include "userwidget.h"

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
    connect(qApp, &QApplication::aboutToQuit, [this]{
        m_hotZoneInterface->EnableZoneDetected(true);
    });
}
void ShutdownManager::shutDownFramegrabKeyboard() {
    this->grabKeyboard();
}
void ShutdownManager::initUI() {
    setObjectName("ShutdownManager");
    resize(qApp->desktop()->screenGeometry().size());

    m_content = new MainFrame(this);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addStretch();
    m_Layout->addWidget(m_content);
    m_Layout->addStretch();

    // hide user switch btn when only 1 user avaliable
    UserWidget *users = new UserWidget;
    if (users->count() < 2)
        m_content->hideBtns(QStringList() << "SwitchUser");
    users->deleteLater();

    QFile qssFile(":/skin/main.qss");
    QString qss;
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        this->setStyleSheet(qss);
        qssFile.close();
    }

    setLayout(m_Layout);
    showFullScreen();
    activateWindow();

    QTimer::singleShot(1000, this, SLOT(shutDownFramegrabKeyboard()));

}

void ShutdownManager::initData() {
    m_sessionInterface = new DBusSessionManagerInterface("com.deepin.SessionManager", "/com/deepin/SessionManager",
                                                         QDBusConnection::sessionBus(), this);
    m_hotZoneInterface = new DBusHotzone("com.deepin.daemon.Zone", "/com/deepin/daemon/Zone",
                                                        QDBusConnection::sessionBus(), this);
    m_hotZoneInterface->EnableZoneDetected(false);
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
    case ShutDownFrame::Shutdown:       m_sessionInterface->RequestShutdown();      break;
    case ShutDownFrame::Restart:        m_sessionInterface->RequestReboot();        break;
    case ShutDownFrame::Suspend:        m_sessionInterface->RequestSuspend();       break;
    case ShutDownFrame::Lock:           m_sessionInterface->RequestLock();          break;
    case ShutDownFrame::Logout:         m_sessionInterface->RequestLogout();        break;
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
    case Qt::Key_Escape:        qApp->quit();  m_hotZoneInterface->EnableZoneDetected(true);  break; // must break;
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
