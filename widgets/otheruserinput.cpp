#include "otheruserinput.h"
#include "util_updateui.h"
#include "constants.h"
#include "keyboardmonitor.h"
#include "public_func.h"

#include <QVBoxLayout>

OtherUserInput::OtherUserInput(QWidget *parent)
    : QFrame(parent)
    , m_capslockMonitor(KeyboardMonitor::instance())
{
    m_capslockMonitor->start(QThread::LowestPriority);

    initUI();
    initConnect();
}

void OtherUserInput::setAlert(const QString &alert)
{
    m_errorTip->setMessage(alert);

    QPoint pos = mapTo(m_errorTip->parentWidget(),
                       QPoint(-20, height() - 10));

    m_errorTip->setCornerPoint(QPoint(30, 20));
    m_errorTip->moveToPos(pos);

    m_errorTip->setVisible(true);
}

void OtherUserInput::setAccount(const QString &username)
{
    m_accountEdit->setText(username);
}

void OtherUserInput::clearAlert() const
{
    m_errorTip->hide();
}

void OtherUserInput::showEvent(QShowEvent *event)
{
    QFrame::showEvent(event);

    m_accountEdit->setFocus();
}

void OtherUserInput::initUI()
{
    m_errorTip = new ErrorTooltip("", this->parentWidget());

    m_errorTip->hide();

    setAttribute(Qt::WA_TranslucentBackground);

    m_accountEdit = new QLineEdit;
    m_passwdEdit = new QLineEdit;
    m_submitBtn = new DImageButton(":/img/action_icons/login_normal.svg",
                                   ":/img/action_icons/login_hover.svg",
                                   ":/img/action_icons/login_press.svg", this);

    m_accountEdit->setObjectName("accountEdit");
    m_passwdEdit->setObjectName("passwdEdit");

    m_passwdEdit->setEchoMode(QLineEdit::Password);
    m_passwdEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

    m_accountEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_accountEdit->setFocusPolicy(Qt::StrongFocus);

    m_passwdEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_passwdEdit->setFocusPolicy(Qt::StrongFocus);

    QHBoxLayout *accountLayout = new QHBoxLayout;
    accountLayout->setMargin(5);
    accountLayout->setSpacing(0);
    accountLayout->addWidget(m_accountEdit);
    QWidget *accountWidget = new QWidget;
    accountWidget->setLayout(accountLayout);
    accountWidget->setObjectName("accountWidget");

    m_capslockWarning = new QLabel(this);
    m_capslockWarning->setVisible(m_capslockMonitor->isCapslockOn());
    m_capslockWarning->setFixedSize(DDESESSIONCC::CapslockWarningWidth,
                                    DDESESSIONCC::CapslockWarningWidth);
    m_capslockWarning->setPixmap(loadPixmap(":/img/capslock.svg"));

    QHBoxLayout *passwdLayout = new QHBoxLayout;
    passwdLayout->setMargin(5);
    passwdLayout->setSpacing(0);
    passwdLayout->addWidget(m_passwdEdit);
    passwdLayout->addSpacing(1);
    passwdLayout->addWidget(m_capslockWarning);
    passwdLayout->addSpacing(1);
    passwdLayout->addWidget(m_submitBtn);

    QWidget *passwdWidget = new QWidget;
    passwdWidget->setLayout(passwdLayout);
    passwdWidget->setObjectName("passwdWidget");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(1);

    layout->addWidget(accountWidget);
    layout->addWidget(passwdWidget);

    setLayout(layout);

    updateStyle(":/skin/passwdedit.qss", this);
}

void OtherUserInput::initConnect()
{
    connect(m_submitBtn, &DImageButton::clicked, this, &OtherUserInput::submit);
    connect(m_passwdEdit, &QLineEdit::returnPressed, this, &OtherUserInput::submit);
    connect(m_accountEdit, &QLineEdit::textEdited, this, &OtherUserInput::clearAlert);
    connect(m_passwdEdit, &QLineEdit::textEdited, this, &OtherUserInput::clearAlert);
    connect(m_capslockMonitor, &KeyboardMonitor::capslockStatusChanged, m_capslockWarning, &QLabel::setVisible);
}
