#include "lockframe.h"
#include "backgroundlabel.h"
#include "timewidget.h"
#include "userwidget.h"
#include "passwdedit.h"
#include "util_updateui.h"
#include "dbus/dbusinputdevices.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QMap>

LockFrame::LockFrame()
{
    setFocusPolicy(Qt::NoFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    setFixedSize(qApp->desktop()->size());

    initUI();
    initBackend();
    updateUI();
    initConnect();

    loadMPRIS();
}
void LockFrame::initConnect() {

    connect(m_passwordEdit, &PassWdEdit::keybdLayoutButtonClicked, m_keybdLayoutWidget, &KbLayoutWidget::show);

}
void LockFrame::initUI() {
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
    m_controlWidget->hideMusicControlWidget();
    m_controlWidget->move(width() - m_controlWidget->width() - 50,
                          height() - m_controlWidget->height() - 36); // margin right 50 margin bottom 36


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

void LockFrame::loadMPRIS()
{
    if (m_mprisInter)
        m_mprisInter->deleteLater();

    QDBusInterface *dbusInter = new QDBusInterface("org.freedesktop.DBus", "/", "org.freedesktop.DBus", QDBusConnection::sessionBus(), this);
    if (!dbusInter)
        return;

    QDBusReply<QStringList> response = dbusInter->call("ListNames");
    const QStringList &serviceList = response.value();
    QString service = QString();
    for (const QString &serv : serviceList)
    {
        if (!serv.startsWith("org.mpris.MediaPlayer2."))
            continue;
        service = serv;
        break;
    }

    if (service.isEmpty())
        return;

    qDebug() << "got service: " << service;

    m_mprisInter = new DBusMediaPlayer2(service, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this);
    m_controlWidget->bindDBusService(m_mprisInter);
    m_controlWidget->showMusicControlWidget();
}

void LockFrame::unlock()
{
    m_userWidget->showLoadingAni();

    qDebug() << "unlock" << m_userWidget->currentUser() << m_passwordEdit->getText();
    const QString &username = m_userWidget->currentUser();
    const QString &password = m_passwordEdit->getText().trimmed();

    QDBusPendingReply<bool> result = m_lockInter->UnlockCheck(username, password);
    result.waitForFinished();

    qDebug() << result.error() << result.value();

    if (result.error().type() == QDBusError::NoError && result.value()) {
        qDebug() << "unLocked quit!";
        qApp->quit();
    }


    // Auth fail
    m_userWidget->hideLoadingAni();
    m_passwordEdit->setAlert(true);
}
void LockFrame::initBackend() {
    m_lockInter = new DBusLockService(LOCKSERVICE_NAME, LOCKSERVICE_PATH, QDBusConnection::systemBus(), this);

    DBusInputDevices * dbusInputDevices = new DBusInputDevices(this);
    foreach (InputDevice device, dbusInputDevices->infos()) {
        if (device.deviceType == "keyboard") {
            m_keyboardLayoutInterface = new DBusKeyboard(this);
            break;
        }
    }

    m_keybdLayoutNameList = m_keyboardLayoutInterface->userLayoutList();

    for (int i = 0; i < m_keybdLayoutNameList.length(); i++) {
        QDBusPendingReply<QString> tmpValue =  m_keyboardLayoutInterface->GetLayoutDesc(m_keybdLayoutNameList[i]);
        tmpValue.waitForFinished();

        keybdLayoutDescList << tmpValue;
        m_keybdInfoMap.insert(m_keybdLayoutNameList[i], tmpValue);
    }
    qDebug() << "QStringList" << m_keybdLayoutNameList;
    m_passwordEdit->updateKeybdLayoutUI(keybdLayoutDescList);
}

void LockFrame::updateUI() {
    m_keybdLayoutWidget = new KbLayoutWidget(keybdLayoutDescList, this);
    m_keybdLayoutWidget->setFixedWidth(DDESESSIONCC::PASSWDLINEEIDT_WIDTH);
    m_keybdLayoutWidget->move(m_passwordEdit->x(), m_passwordEdit->y() + m_passwordEdit->height() + 10);
    m_keybdLayoutWidget->hide();

    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, this, &LockFrame::setCurrentKeyboardLayout);
    connect(m_keybdLayoutWidget, &KbLayoutWidget::setButtonClicked, &KbLayoutWidget::hide);
}

void LockFrame::setCurrentKeyboardLayout(QString keyboard_value) {
    QMap<QString, QString>::iterator lookupIt;

    for (lookupIt = m_keybdInfoMap.begin(); lookupIt != m_keybdInfoMap.end(); ++lookupIt) {
        if (lookupIt.value() == keyboard_value && m_keyboardLayoutInterface->currentLayout() != lookupIt.key()) {

            m_keyboardLayoutInterface->setCurrentLayout(lookupIt.key());
        }
    }

}


