#include <QtCore/QObject>
#include <QDebug>
#include "passwdedit.h"

PassWdEdit::PassWdEdit(QString iconId, QWidget* parent)
    : QFrame(parent)
{
    m_iconId = iconId;
    initUI();
    initConnect();

    setObjectName("passwdEnterFrame");
    setFixedSize(QSize(270, 36));
    setKeyBoardLayoutButtonStatus(false);
}

PassWdEdit::~PassWdEdit()
{

}

void PassWdEdit::initUI() {
    m_keyboardButton = new QPushButton;
    m_keyboardButton->setObjectName("KeyBoardLayoutButton");
    m_keyboardButton->setFixedSize(QSize(24, 24));
    m_iconButton = new QPushButton;
    m_iconButton->setObjectName(m_iconId);
    m_iconButton->setCheckable(true);
    m_iconButton->setFixedSize(QSize(34, 34)); // 34 = 36 - borderTop - borderBottom
    m_iconButton->setIconSize(QSize(34, 34));

    m_lineEdit = new QLineEdit;
    m_lineEdit->setObjectName("passwdLineEdit");
    m_lineEdit->setEchoMode(QLineEdit::Password);
    m_lineEdit->setFixedHeight(m_iconButton->height());
    m_lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addWidget(m_keyboardButton);
    m_Layout->addWidget(m_lineEdit);
    m_Layout->addStretch();
    m_Layout->addWidget(m_iconButton);

    setLayout(m_Layout);
}
void PassWdEdit::initConnect() {
    connect(m_iconButton, &QPushButton::clicked, this, &PassWdEdit::jumpTo);
    connect(this, &PassWdEdit::jumpTo, this, &PassWdEdit::testing);
}

void PassWdEdit::focusInEvent(QFocusEvent *)
{
    m_lineEdit->setFocus();
}

void PassWdEdit::testing() {
    qDebug() << "testing:";
}

void PassWdEdit::setKeyBoardLayoutButtonStatus(bool show) {
    if (show) {
        m_keyboardButton->show();
        setFixedWidth(270);
    } else {
        m_keyboardButton->hide();
        setFixedWidth(240);
    }
}

void PassWdEdit::setLineEditRightImage(QString imageUrl) {
    m_iconButton->setIcon(QIcon(QPixmap(imageUrl)));
}
QString PassWdEdit::getText() {
    return m_lineEdit->text();
}
