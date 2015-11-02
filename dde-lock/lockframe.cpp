#include "lockframe.h"
#include "backgroundlabel.h"
#include "timewidget.h"
#include "userwidget.h"
#include "passwdedit.h"
#include "util_updateui.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDebug>

LockFrame::LockFrame()
{
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    setFixedSize(qApp->desktop()->size());

    // background image
    BackgroundLabel *background = new BackgroundLabel(true, this);
    background->move(0, 0);
    TimeWidget *timeWidget = new TimeWidget(this);
    timeWidget->setFixedSize(400, 300);
//    timeWidget->setStyleSheet("background-color:red;");
    timeWidget->move(48, qApp->desktop()->height() - timeWidget->height() - 36); // left 48px and bottom 36px
    m_userWidget = new UserWidget(this);
//    m_userWidget->hide();
    m_userWidget->move(0, (qApp->desktop()->height() - m_userWidget->height()) / 2 - 95);
    m_passwordEdit = new PassWdEdit("LockIcon", this);
    m_passwordEdit->setFocusPolicy(Qt::StrongFocus);
    m_passwordEdit->setFocus();

    m_controlWidget = new ControlWidget(this);
    m_controlWidget->move(width() - m_controlWidget->width() - 50,
                          height() - m_controlWidget->height() - 36); // margin right 50 margin bottom 36

    m_lockInter = new DBusLockService("com.deepin.dde.lock", "/com/deepin/dde/lock", QDBusConnection::systemBus(), this);

    QHBoxLayout *passwdLayout = new QHBoxLayout;
    passwdLayout->setMargin(0);
    passwdLayout->setSpacing(0);
    passwdLayout->addStretch();
    passwdLayout->addWidget(m_passwordEdit);
    passwdLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addStretch();
    mainLayout->addLayout(passwdLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
    showFullScreen();
    activateWindow();
    updateStyle(":/theme/theme/lock.qss", this);

    connect(m_passwordEdit, &PassWdEdit::submit, this, &LockFrame::unlock);
}

void LockFrame::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    default:;
    }
}

void LockFrame::unlock()
{
    qDebug() << "unlock" << m_userWidget->currentUser() << m_passwordEdit->getText();
    const QString &username = m_userWidget->currentUser();
    const QString &password = m_passwordEdit->getText().trimmed();

    QDBusPendingReply<bool> result = m_lockInter->UnlockCheck(username, password);
    result.waitForFinished();

    qDebug() << result.error() << result.value();

    if (result.error().type() == QDBusError::NoError && result.value())
        qApp->quit();

    // TODO: auth fail
}
