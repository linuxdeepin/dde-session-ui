#include "userinputwidget.h"

#include "constants.h"
#include "util_updateui.h"

#include <QVBoxLayout>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>

#if 0
QT_TRANSLATE_NOOP("UserInputWidget", "Login")
#endif

UserInputWidget::UserInputWidget(QWidget *parent)
    : QWidget(parent)
    , m_userAvatar(new UserAvatar(this))
    , m_nameLbl(new QLabel(this))
    , m_passwordEdit(new DPasswdEditAnimated(this))
    , m_loginBtn(new QPushButton(this))
{
    std::function<void (QString)> loginTr = std::bind(&QPushButton::setText, m_loginBtn, std::placeholders::_1);
    m_trList.push_back(std::pair<std::function<void (QString)>, QString>(loginTr, "Login"));

    m_userAvatar->setAvatarSize(UserAvatar::AvatarLargeSize);
    m_userAvatar->setFixedSize(100, 100);

    m_nameLbl->setStyleSheet("color: white;");

    m_passwordEdit->setSubmitIcon(":/img/action_icons/unlock_normal.svg",
                                  ":/img/action_icons/unlock_hover.svg",
                                  ":/img/action_icons/unlock_press.svg");

    m_passwordEdit->setEyeButtonEnable(false);
    m_passwordEdit->setContentsMargins(5, 0, 0, 0);
    m_passwordEdit->lineEdit()->setContextMenuPolicy(Qt::NoContextMenu);
    m_passwordEdit->setFixedSize(QSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH, DDESESSIONCC::PASSWDLINEEDIT_HEIGHT));
    m_passwordEdit->setFocusPolicy(Qt::StrongFocus);
    // FIXME: do not work in qss
    m_passwordEdit->invalidMessage()->setStyleSheet("#InvalidMessage{color: #f9704f;}");
    updateStyle(":/skin/dpasswdeditanimated.qss", m_passwordEdit);

    m_loginBtn->setFixedHeight(DDESESSIONCC::PASSWDLINEEDIT_HEIGHT);

    m_passwordEdit->setVisible(true);
    m_passwordEdit->setFocus();
    m_passwordEdit->lineEdit()->setAttribute(Qt::WA_InputMethodEnabled, false);

    m_nameLbl->setFixedSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH, 25);
    m_nameLbl->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addStretch();
    layout->addWidget(m_passwordEdit, 0, Qt::AlignHCenter);
    layout->addWidget(m_loginBtn, 0, Qt::AlignHCenter);
    layout->addStretch();

    setFixedHeight((m_userAvatar->height() + 20 + m_nameLbl->height() + 18) * 2 + DDESESSIONCC::PASSWDLINEEDIT_HEIGHT);
    m_loginBtn->hide();

    setLayout(layout);

    connect(m_passwordEdit, &DPasswdEditAnimated::submit, this, &UserInputWidget::requestAuthUser);
    connect(m_loginBtn, &QPushButton::clicked, this, [=] {
        emit requestAuthUser(QString());
    });

    refreshLanguage();
    refreshAvatarPosition();
}

void UserInputWidget::setName(const QString &name)
{
    m_nameLbl->setText(name);
}

void UserInputWidget::setAvatar(const QString &avatar)
{
    m_userAvatar->setIcon(avatar);
}

void UserInputWidget::setIsNoPasswordGrp(bool isNopassword)
{
    m_passwordEdit->setVisible(!isNopassword);
    m_loginBtn->setVisible(isNopassword);
}

void UserInputWidget::setFaildMessage(const QString &message)
{
    m_passwordEdit->lineEdit()->setPlaceholderText(message);
}

void UserInputWidget::setFaildTipMessage(const QString &message)
{
    m_passwordEdit->showAlert(message);
}

void UserInputWidget::shutdownMode()
{
    m_passwordEdit->setSubmitIcon(":/img/action_icons/shutdown_normal.svg",
                                  ":/img/action_icons/shutdown_hover.svg",
                                  ":/img/action_icons/shutdown_press.svg");
    m_passwordEdit->showAlert(tr("Enter your password to shutdown"));
    m_loginBtn->setText(QApplication::translate("ShutdownWidget", "Shut down"));
}

void UserInputWidget::normalMode()
{
    m_passwordEdit->setSubmitIcon(":/img/action_icons/unlock_normal.svg",
                                  ":/img/action_icons/unlock_hover.svg",
                                  ":/img/action_icons/unlock_press.svg");
    m_passwordEdit->hideAlert();
    m_loginBtn->setText(QApplication::translate("ShutdownWidget", "Login"));
}

void UserInputWidget::restartMode()
{
    m_passwordEdit->setSubmitIcon(":/img/action_icons/reboot_normal.svg",
                                  ":/img/action_icons/reboot_hover.svg",
                                  ":/img/action_icons/reboot_press.svg");
    m_passwordEdit->showAlert(tr("Enter your password to reboot"));
    m_loginBtn->setText(QApplication::translate("ShutdownWidget", "Reboot"));
}

void UserInputWidget::grabKeyboard()
{
    m_passwordEdit->lineEdit()->grabKeyboard();
}

void UserInputWidget::releaseKeyboard()
{
    m_passwordEdit->lineEdit()->releaseKeyboard();
}

bool UserInputWidget::event(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        refreshLanguage();
    }

    if (event->type() == QEvent::Show) {
        QTimer::singleShot(400, this, [=] {
            m_passwordEdit->lineEdit()->setFocus();
        });
    }

    return QWidget::event(event);
}

void UserInputWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit abortOperation();
    }

    return QWidget::keyPressEvent(event);
}

void UserInputWidget::resizeEvent(QResizeEvent *event)
{
    QTimer::singleShot(0, this, &UserInputWidget::refreshAvatarPosition);

    return QWidget::resizeEvent(event);
}

void UserInputWidget::refreshLanguage()
{
    for (auto it = m_trList.begin(); it != m_trList.end(); ++it) {
        it->first(it->second.toUtf8());
    }
}

void UserInputWidget::refreshAvatarPosition()
{
    m_userAvatar->move((width() - m_userAvatar->width()) / 2, 0);
    m_nameLbl->move((width() - m_nameLbl->width()) / 2, m_userAvatar->height() + 20);
}
