#include "userinputwidget.h"

#include "constants.h"
#include "util_updateui.h"
#include "kblayoutwidget.h"

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
    , m_otherUserInput(new OtherUserInput(this))
    , m_loginBtn(new LoginButton(this))
    , m_kbLayoutBorder(new DArrowRectangle(DArrowRectangle::ArrowTop))
    , m_kbLayoutWidget(new KbLayoutWidget(QStringList()))
    , m_lockPasswordWidget(new LockPasswordWidget)
{
    std::function<void (QString)> loginTr = std::bind(&LoginButton::setText, m_loginBtn, std::placeholders::_1);
    m_trList.push_back(std::pair<std::function<void (QString)>, QString>(loginTr, "Login"));

    QMap<QString, int> registerFunctionIndexs;
    std::function<void (QVariant)> passwordChanged = std::bind(&UserInputWidget::onOtherPagePasswordChanged, this, std::placeholders::_1);
    registerFunctionIndexs["Password"] = FrameDataBind::Instance()->registerFunction("Password", passwordChanged);

    std::function<void (QVariant)> kblayoutChanged = std::bind(&UserInputWidget::onOtherPageKBLayoutChanged, this, std::placeholders::_1);
    registerFunctionIndexs["KBLayout"] = FrameDataBind::Instance()->registerFunction("KBLayout", kblayoutChanged);

    connect(this, &UserInputWidget::destroyed, this, [=] {
        for (auto it = registerFunctionIndexs.constBegin(); it != registerFunctionIndexs.constEnd(); ++it) {
            FrameDataBind::Instance()->unRegisterFunction(it.key(), it.value());
        }
    });

    QTimer::singleShot(0, this, [=] {
        FrameDataBind::Instance()->refreshData("Password");
        FrameDataBind::Instance()->refreshData("KBLayout");
    });

    m_userAvatar->setAvatarSize(UserAvatar::AvatarLargeSize);
    m_userAvatar->setFixedSize(100, 100);

    m_nameLbl->setStyleSheet("font-size: 16px;"
                             "font-weight: normal;"
                             "font-style: normal;"
                             "line-height: normal;"
                             "text-align: center;"
                             "color: #ffffff;");

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
    m_loginBtn->setFixedWidth(128);
    m_loginBtn->setFocusPolicy(Qt::StrongFocus);

    m_passwordEdit->setVisible(true);
    m_passwordEdit->setFocus();
    m_passwordEdit->lineEdit()->setAttribute(Qt::WA_InputMethodEnabled, false);

    m_nameLbl->setFixedSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH, 25);
    m_nameLbl->setAlignment(Qt::AlignCenter);

    m_lockPasswordWidget->setFixedSize(QSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH, DDESESSIONCC::PASSWDLINEEDIT_HEIGHT));

    QVBoxLayout *layout = new QVBoxLayout;

    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addStretch();
    // 因为主窗口布局下面的工具栏高度为132，为了输入框在主窗口上看着是垂直居中对齐
    layout->addSpacing(132);
    layout->addWidget(m_otherUserInput, 0, Qt::AlignHCenter);
    layout->addWidget(m_passwordEdit, 0, Qt::AlignHCenter);
    layout->addWidget(m_lockPasswordWidget, 0, Qt::AlignHCenter);
    layout->addWidget(m_loginBtn, 0, Qt::AlignHCenter);
    layout->addStretch();

    setLayout(layout);

    m_userAvatar->hide();
    m_nameLbl->hide();
    m_passwordEdit->hide();
    m_otherUserInput->hide();
    m_loginBtn->hide();
    m_kbLayoutBorder->hide();
    m_lockPasswordWidget->hide();

    m_kbLayoutBorder->setBackgroundColor(QColor(255, 255, 255, 255 * 0.2));
    m_kbLayoutBorder->setBorderColor(QColor(0, 0, 0, 0));
    m_kbLayoutBorder->setBorderWidth(0);
    m_kbLayoutBorder->setMargin(0);

    m_kbLayoutBorder->setContent(m_kbLayoutWidget);
    m_kbLayoutBorder->setFixedWidth(DDESESSIONCC::PASSWDLINEEIDT_WIDTH);

    m_kbLayoutWidget->setFixedWidth(DDESESSIONCC::PASSWDLINEEIDT_WIDTH);

    m_otherUserInput->setFixedWidth(DDESESSIONCC::PASSWDLINEEIDT_WIDTH);
    m_otherUserInput->setFixedHeight(DDESESSIONCC::PASSWDLINEEDIT_HEIGHT * 2);

    connect(m_passwordEdit, &DPasswdEditAnimated::keyboardButtonClicked, this, &UserInputWidget::toggleKBLayoutWidget);
    connect(m_passwordEdit->lineEdit(), &QLineEdit::textChanged, this, [=] (const QString &value) {
        FrameDataBind::Instance()->updateValue("Password", value);
    });
    connect(m_passwordEdit, &DPasswdEditAnimated::submit, this, [=] (const QString &passwd) {
        if (passwd.isEmpty()) return;

        emit requestAuthUser(passwd);
    });
    connect(m_loginBtn, &LoginButton::clicked, this, [=] {
        emit requestAuthUser(QString());
    });

    connect(m_otherUserInput, &OtherUserInput::submit, this, [=] {
        if (ADDomainUser *user = dynamic_cast<ADDomainUser*>(m_user.get())) {
           user->setUserName(m_otherUserInput->account());
           emit requestAuthUser(m_otherUserInput->passwd());
        }
        else {
            qWarning() << "unknow error, check dynamic_cast<ADDomainUser>.";
        }
    });

    connect(m_kbLayoutWidget, &KbLayoutWidget::setButtonClicked, this, &UserInputWidget::requestUserKBLayoutChanged);
    connect(m_kbLayoutWidget, &KbLayoutWidget::focusOuted, m_kbLayoutBorder, &DArrowRectangle::hide);

    refreshLanguage();
    refreshAvatarPosition();
}

UserInputWidget::~UserInputWidget()
{
    // 因为键盘布局控件和UserInputWidget没有父子关系
    // 此处需要手动销毁对象
    m_kbLayoutBorder->deleteLater();
}

void UserInputWidget::setUser(std::shared_ptr<User> user)
{
    for (auto connect : m_currentUserConnects) {
        m_user->disconnect(connect);
    }

    if (m_user.get() != nullptr) {
        m_passwords[m_user->uid()] = m_passwordEdit->lineEdit()->text();
    }

    m_passwordEdit->lineEdit()->setText(m_passwords[user->uid()]);

    m_currentUserConnects.clear();

    m_currentUserConnects << connect(user.get(), &User::kbLayoutListChanged, this, &UserInputWidget::updateKBLayout, Qt::UniqueConnection);
    m_currentUserConnects << connect(user.get(), &User::currentKBLayoutChanged, this, &UserInputWidget::setDefaultKBLayout, Qt::UniqueConnection);
    m_currentUserConnects << connect(user.get(), &User::lockChanged, this, &UserInputWidget::disablePassword);
    m_currentUserConnects << connect(user.get(), &User::avatarChanged, this, &UserInputWidget::setAvatar);
    m_currentUserConnects << connect(user.get(), &User::displayNameChanged, this, &UserInputWidget::setName);

    m_user = user;

    setName(user->displayName());
    setAvatar(user->avatarPath());
    disablePassword(user.get()->isLock());
    m_loginBtn->hide();
    m_passwordEdit->hide();
    m_otherUserInput->hide();

    refreshInputState();
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
    m_passwordEdit->abortAuth();
    m_passwordEdit->setVisible(!isNopassword);
    m_loginBtn->setVisible(isNopassword);
}

void UserInputWidget::setFaildMessage(const QString &message)
{
    m_passwordEdit->abortAuth();
    if (m_lockPasswordWidget->isVisible()) {
        m_lockPasswordWidget->setMessage(message);
        return;
    }
    m_passwordEdit->lineEdit()->setPlaceholderText(message);
}

void UserInputWidget::setFaildTipMessage(const QString &message)
{
    if (message.isEmpty()) {
        m_passwordEdit->hideAlert();
    }
    else {
        m_passwordEdit->showAlert(message);
    }
}

void UserInputWidget::updateKBLayout(const QStringList &list)
{
    m_passwordEdit->setKeyboardButtonEnable(list.size() > 1);
    m_kbLayoutWidget->updateButtonList(list);
    m_kbLayoutBorder->setContent(m_kbLayoutWidget);
}

void UserInputWidget::setDefaultKBLayout(const QString &layout)
{
    m_kbLayoutWidget->setDefault(layout);
}

void UserInputWidget::disablePassword(bool disable)
{
    m_passwordEdit->lineEdit()->setDisabled(disable);
    m_passwordEdit->setVisible(!disable);
    m_lockPasswordWidget->setVisible(disable);

    if (disable) {
        setFaildMessage(tr("Please try again %n minute(s) later", "", m_user->lockNum()));
    }
}

void UserInputWidget::updateAuthType(SessionBaseModel::AuthType type)
{
    m_authType = type;

    if (type == SessionBaseModel::AuthType::LightdmType) {
        m_passwordEdit->setSubmitIcon(":/img/action_icons/login_normal.svg",
                                      ":/img/action_icons/login_hover.svg",
                                      ":/img/action_icons/login_press.svg");
    }
}

void UserInputWidget::resetAllState()
{
    m_passwordEdit->lineEdit()->clear();
    normalMode();
    m_passwordEdit->abortAuth();
}

void UserInputWidget::shutdownMode()
{
    m_passwordEdit->setSubmitIcon(":/img/action_icons/shutdown_normal.svg",
                                  ":/img/action_icons/shutdown_hover.svg",
                                  ":/img/action_icons/shutdown_press.svg");
    m_passwordEdit->showAlert(tr("Enter your password to shut down"));
    m_loginBtn->setText(QApplication::translate("ShutdownWidget", "Shut down"));
}

void UserInputWidget::normalMode()
{
    if (m_authType == SessionBaseModel::AuthType::LightdmType) {
        m_passwordEdit->setSubmitIcon(":/img/action_icons/login_normal.svg",
                                      ":/img/action_icons/login_hover.svg",
                                      ":/img/action_icons/login_press.svg");
    }
    else {
        m_passwordEdit->setSubmitIcon(":/img/action_icons/unlock_normal.svg",
                                      ":/img/action_icons/unlock_hover.svg",
                                      ":/img/action_icons/unlock_press.svg");
    }

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
    if (m_passwordEdit->isVisible()) {
        m_passwordEdit->lineEdit()->grabKeyboard();
        return;
    }

    if (m_loginBtn->isVisible()) {
        m_loginBtn->grabKeyboard();
        m_loginBtn->setFocus();
        return;
    }

    // other input
    if (m_otherUserInput->isVisible()) {
        m_passwordEdit->lineEdit()->releaseKeyboard();
        m_otherUserInput->grabKeyboard();
        return;
    }
}

void UserInputWidget::releaseKeyboard()
{
    m_passwordEdit->lineEdit()->releaseKeyboard();
    m_loginBtn->releaseKeyboard();
    m_otherUserInput->releaseKeyboard();
}

void UserInputWidget::hideKeyboard()
{
    m_kbLayoutBorder->hide();
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
    QTimer::singleShot(0, this, &UserInputWidget::refreshKBLayoutWidgetPosition);
    QTimer::singleShot(0, m_userAvatar, &QWidget::show);
    QTimer::singleShot(0, m_nameLbl, &QWidget::show);

    return QWidget::resizeEvent(event);
}

void UserInputWidget::showEvent(QShowEvent *event)
{
    refreshInputState();

    return QWidget::showEvent(event);
}

void UserInputWidget::refreshLanguage()
{
    for (auto it = m_trList.begin(); it != m_trList.end(); ++it) {
        it->first(qApp->translate("UserInputWidget", it->second.toUtf8()));
    }
}

void UserInputWidget::refreshAvatarPosition()
{
    QWidget *first_visible_widget = nullptr;

    for (int i = 0; i < layout()->count(); ++i) {
        if (QWidget *w = layout()->itemAt(i)->widget()) {
            if (w->isVisible()) {
                first_visible_widget = w;
                break;
            }
        }
    }

    int bottom_y = first_visible_widget ? first_visible_widget->y() : height() / 2;

    m_nameLbl->move((width() - m_nameLbl->width()) / 2, bottom_y - m_nameLbl->height() - 20);
    m_userAvatar->move((width() - m_userAvatar->width()) / 2, m_nameLbl->y() - m_userAvatar->height());
}

void UserInputWidget::toggleKBLayoutWidget()
{
    if (m_kbLayoutBorder->isVisible()) {
        m_kbLayoutBorder->hide();
    }
    else {
        // 保证布局选择控件不会被其它控件遮挡
        // 必须要将它作为主窗口的子控件
        m_kbLayoutBorder->setParent(window());
        m_kbLayoutBorder->setVisible(true);
        m_kbLayoutBorder->raise();
        refreshKBLayoutWidgetPosition();
    }

    FrameDataBind::Instance()->updateValue("KBLayout", m_kbLayoutBorder->isVisible());
}

void UserInputWidget::refreshKBLayoutWidgetPosition()
{
    const QPoint &point = mapTo(m_kbLayoutBorder->parentWidget(), QPoint(m_passwordEdit->geometry().x() + (m_passwordEdit->width() / 2),
                                                                         m_passwordEdit->geometry().bottomLeft().y() - 15));
    m_kbLayoutBorder->move(point.x(), point.y());
    m_kbLayoutBorder->setArrowX(15);
}

void UserInputWidget::refreshInputState()
{
    int frameHeight = (m_userAvatar->height() + 20 + m_nameLbl->height() + 18) * 2;

    if (m_user->type() == User::ADDomain && m_user->uid() == 0) {
        m_passwordEdit->hide();
        m_otherUserInput->show();
        frameHeight += m_otherUserInput->height();
        m_otherUserInput->setFocus();
    }
    else if (m_user->type() == User::ADDomain) {
        m_passwordEdit->show();
        frameHeight += DDESESSIONCC::PASSWDLINEEDIT_HEIGHT;
        grabKeyboard();
    }
    else {
        setIsNoPasswordGrp(m_user->isNoPasswdGrp());
        updateKBLayout(m_user->kbLayoutList());
        setDefaultKBLayout(m_user->currentKBLayout());
        frameHeight += DDESESSIONCC::PASSWDLINEEDIT_HEIGHT;
    }

    setFixedHeight(frameHeight);
}

void UserInputWidget::onOtherPagePasswordChanged(const QVariant &value)
{
    m_passwordEdit->lineEdit()->setText(value.toString());
}

void UserInputWidget::onOtherPageKBLayoutChanged(const QVariant &value)
{
    if (value.toBool()) {
        m_kbLayoutBorder->setParent(window());
    }

    m_kbLayoutBorder->setVisible(value.toBool());

    if (m_kbLayoutBorder->isVisible())
        m_kbLayoutBorder->raise();

    refreshKBLayoutWidgetPosition();
}
