#include "otheruserinput.h"
#include "util_updateui.h"
#include "constants.h"

#include <QVBoxLayout>

OtherUserInput::OtherUserInput(QWidget *parent)
    : QFrame(parent)
{
    initUI();
}

void OtherUserInput::setAlert(const QString &alert)
{
    if (alert.isEmpty()) {
        m_errorTip->hide();
    } else {
        m_errorTip->setMessage(alert);

        QPoint pos = mapTo(m_errorTip->parentWidget(),
                           QPoint(-20, height() - 10));

        m_errorTip->setCornerPoint(QPoint(30, height() - 13));
        m_errorTip->moveToPos(pos);
        m_errorTip->setVisible(true);
    }
}

void OtherUserInput::initUI()
{
    m_errorTip = new ErrorTooltip("", this->parentWidget());

    m_errorTip->hide();

    setAttribute(Qt::WA_TranslucentBackground);

    setFixedWidth(DDESESSIONCC::PASSWDLINEEIDT_WIDTH - 2);

    m_accountEdit = new QLineEdit;
    m_passwdEdit = new QLineEdit;
    m_submitBtn = new DImageButton(":/img/action_icons/unlock_normal.svg",
                                   ":/img/action_icons/unlock_hover.svg",
                                   ":/img/action_icons/unlock_press.svg", this);

    m_accountEdit->setObjectName("accountEdit");
    m_passwdEdit->setObjectName("passwdEdit");

    m_passwdEdit->setEchoMode(QLineEdit::Password);
    m_passwdEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

    QHBoxLayout *accountLayout = new QHBoxLayout;
    accountLayout->setMargin(5);
    accountLayout->setSpacing(0);
    accountLayout->addWidget(m_accountEdit);
    QWidget *accountWidget = new QWidget;
    accountWidget->setLayout(accountLayout);
    accountWidget->setObjectName("accountWidget");

    QHBoxLayout *passwdLayout = new QHBoxLayout;
    passwdLayout->setMargin(5);
    passwdLayout->setSpacing(0);
    passwdLayout->addWidget(m_passwdEdit);
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
}
