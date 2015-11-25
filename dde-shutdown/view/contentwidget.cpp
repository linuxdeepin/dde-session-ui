#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtCore/QObject>

#include "dbus/dbusvariant.h"
#include "contentwidget.h"

ShutDownFrame::ShutDownFrame(QWidget *parent)
    : QFrame(parent)
{
    initUI();
    initConnect();
}

ShutDownFrame::~ShutDownFrame()
{

}

void ShutDownFrame::initConnect() {
    connect(this, SIGNAL(keyLeft()), SLOT(setPreviousChildFocus()));
    connect(this, SIGNAL(keyRight()), SLOT(setNextChildFocus()));
    connect(this, &ShutDownFrame::pressEnterAction, this, &ShutDownFrame::enterKeyPushed);

    connect(m_shutdownButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Shutdown);});
    connect(m_restartButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Restart);});
    connect(m_suspendButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Suspend);});
    connect(m_lockButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Lock);});
    connect(m_logoutButton, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(Logout);});
    connect(m_switchUserBtn, &RoundItemButton::clicked, [this] {emit ShutDownFrameActions(SwitchUser);});
}

void ShutDownFrame::enterKeyPushed()
{
    if (m_currentSelectedBtn->isDisabled())
        return;

    if (m_currentSelectedBtn == m_shutdownButton)
        emit ShutDownFrameActions(Shutdown);
    else if (m_currentSelectedBtn == m_restartButton)
        emit ShutDownFrameActions(Restart);
    else if (m_currentSelectedBtn == m_suspendButton)
        emit ShutDownFrameActions(Suspend);
    else if (m_currentSelectedBtn == m_lockButton)
        emit ShutDownFrameActions(Lock);
    else if (m_currentSelectedBtn == m_logoutButton)
        emit ShutDownFrameActions(Logout);
    else if (m_currentSelectedBtn == m_switchUserBtn)
        emit ShutDownFrameActions(SwitchUser);
}

void ShutDownFrame::hideBtn(const QString &btnName)
{
    if (!btnName.compare("Shutdown", Qt::CaseInsensitive))
        m_shutdownButton->hide();
    else if (!btnName.compare("Restart", Qt::CaseInsensitive))
        m_restartButton->hide();
    else if (!btnName.compare("Suspend", Qt::CaseInsensitive))
        m_suspendButton->hide();
    else if (!btnName.compare("Lock", Qt::CaseInsensitive))
        m_lockButton->hide();
    else if (!btnName.compare("Logout", Qt::CaseInsensitive))
        m_logoutButton->hide();
    else if (!btnName.compare("SwitchUser", Qt::CaseInsensitive))
        m_switchUserBtn->hide();
    else
        return;
}

void ShutDownFrame::disableBtn(const QString &btnName)
{
    if (!btnName.compare("Shutdown", Qt::CaseInsensitive))
        m_shutdownButton->setDisabled(true);
    else if (!btnName.compare("Restart", Qt::CaseInsensitive))
        m_restartButton->setDisabled(true);
    else if (!btnName.compare("Suspend", Qt::CaseInsensitive))
        m_suspendButton->setDisabled(true);
    else if (!btnName.compare("Lock", Qt::CaseInsensitive))
        m_lockButton->setDisabled(true);
    else if (!btnName.compare("Logout", Qt::CaseInsensitive))
        m_logoutButton->setDisabled(true);
    else if (!btnName.compare("SwitchUser", Qt::CaseInsensitive))
        m_switchUserBtn->setDisabled(true);
    else
        return;
}

void ShutDownFrame::initUI() {
    m_btnsList = new QList<RoundItemButton *>;
    m_shutdownButton = new RoundItemButton(tr("Shut down"));
    m_shutdownButton->setAutoExclusive(true);
    m_shutdownButton->setObjectName("ShutDownButton");
    m_restartButton = new RoundItemButton(tr("Restart"));
    m_restartButton->setAutoExclusive(true);
    m_restartButton->setObjectName("RestartButton");
    m_suspendButton = new RoundItemButton(tr("Suspend"));
    m_suspendButton->setAutoExclusive(true);
    m_suspendButton->setObjectName("SuspendButton");
    m_lockButton = new RoundItemButton(tr("Lock"));
    m_lockButton->setAutoExclusive(true);
    m_lockButton->setObjectName("LockButton");
    m_logoutButton = new RoundItemButton(tr("Log out"));
    m_logoutButton->setAutoExclusive(true);
    m_logoutButton->setObjectName("LogoutButton");

    m_switchUserBtn = new RoundItemButton(tr("Switch user"));
    m_switchUserBtn->setAutoExclusive(true);
    m_switchUserBtn->setObjectName("SwitchUserButton");

    QLabel *tipsIcon = new QLabel;
    tipsIcon->setPixmap(QPixmap(":/img/waring.png"));
    m_tipsLabel = new QLabel;
    m_tipsLabel->setAlignment(Qt::AlignCenter);
    m_tipsLabel->setStyleSheet("color:white;"
                               "font-size:14px;");
    QHBoxLayout *tipsLayout = new QHBoxLayout;
    tipsLayout->addStretch();
    tipsLayout->addWidget(tipsIcon);
    tipsLayout->addWidget(m_tipsLabel);
    tipsLayout->addStretch();

    m_tipsWidget = new QWidget;
    m_tipsWidget->hide();
    m_tipsWidget->setLayout(tipsLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_shutdownButton);
    buttonLayout->addWidget(m_restartButton);
    buttonLayout->addWidget(m_suspendButton);
    buttonLayout->addWidget(m_lockButton);
    buttonLayout->addWidget(m_switchUserBtn);
    buttonLayout->addWidget(m_logoutButton);
    buttonLayout->addStretch(0);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(m_tipsWidget);
    mainLayout->addStretch();
    setFocusPolicy(Qt::StrongFocus);
    setLayout(mainLayout);

    updateStyle(":/skin/shutdown.qss", this);


    m_btnsList->append(m_shutdownButton);
    m_btnsList->append(m_restartButton);
    m_btnsList->append(m_suspendButton);
    m_btnsList->append(m_lockButton);
    m_btnsList->append(m_switchUserBtn);
    m_btnsList->append(m_logoutButton);

    m_currentSelectedBtn = m_shutdownButton;
    m_currentSelectedBtn->updateState(RoundItemButton::Default);

    //// Inhibit to shutdown
    inhibitShutdown();

    QTimer* checkTooltip = new QTimer(this);
    checkTooltip->setInterval(5*60*1000);
    checkTooltip->start();
    connect(checkTooltip,  &QTimer::timeout, this, &ShutDownFrame::inhibitShutdown);
}

void ShutDownFrame::inhibitShutdown() {
    m_login1Inter = new DBusLogin1Manager("org.freedesktop.login1", "/org/freedesktop/login1", QDBusConnection::systemBus(), this);
    QString reminder_tooltip  = QString();

    if (m_login1Inter->isValid()) {
        qDebug() <<  "m_login1Inter is valid!";

        QDBusPendingReply<InhibitorsList> reply = m_login1Inter->ListInhibitors();
        reply.waitForFinished();

        if (!reply.isError()){
            InhibitorsList inhibitList = qdbus_cast<InhibitorsList>(reply.argumentAt(0));
            qDebug() << "inhibitList:" << inhibitList.count();

            for(int i = 0; i < inhibitList.count();i++) {
                if (inhibitList.at(i).what == "shutdown" && inhibitList.at(i).dosome == "block") {
                    reminder_tooltip = inhibitList.at(i).why;
                }
            }
            qDebug() << "shutdown Reason!" << reminder_tooltip;
            showTips(reminder_tooltip);
        } else {
            reminder_tooltip = "";
            qDebug() << reply.error().message();
        }
    } else {
        qDebug() << "shutdown login1Manager error!";
        reminder_tooltip = "";
    }
}

void ShutDownFrame::setPreviousChildFocus()
{
    if (!m_currentSelectedBtn->isDisabled() &&
        !m_currentSelectedBtn->isChecked())
        m_currentSelectedBtn->updateState(RoundItemButton::Normal);

    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    const int nextPos = lastPos ? lastPos : m_btnsList->count();

    m_currentSelectedBtn = m_btnsList->at(nextPos - 1);

    if (m_currentSelectedBtn->isDisabled() || !m_currentSelectedBtn->isVisible())
        setPreviousChildFocus();
    else
        m_currentSelectedBtn->setChecked(true);
}

void ShutDownFrame::setNextChildFocus()
{
    if (!m_currentSelectedBtn->isDisabled() &&
        !m_currentSelectedBtn->isChecked())
        m_currentSelectedBtn->updateState(RoundItemButton::Normal);

    const int lastPos = m_btnsList->indexOf(m_currentSelectedBtn);
    m_currentSelectedBtn = m_btnsList->at((lastPos + 1) % m_btnsList->count());

    if (m_currentSelectedBtn->isDisabled() || !m_currentSelectedBtn->isVisible())
        setNextChildFocus();
    else
        m_currentSelectedBtn->setChecked(true);
}

void ShutDownFrame::showTips(const QString &tips)
{
    if (!tips.isEmpty()) {
        m_tipsLabel->setText(tips);
        m_tipsWidget->show();
        m_shutdownButton->setDisabled(true);
        m_restartButton->setDisabled(true);
        m_suspendButton->setDisabled(true);
    } else {
        m_tipsWidget->hide();
        m_shutdownButton->setDisabled(false);
        m_restartButton->setDisabled(false);
        m_suspendButton->setDisabled(false);
    }
}

void ShutDownFrame::hideBtns(const QStringList &btnsName)
{
    for (const QString &name : btnsName)
        hideBtn(name);
}

void ShutDownFrame::disableBtns(const QStringList &btnsName)
{
    for (const QString &name : btnsName)
        disableBtn(name);
}
