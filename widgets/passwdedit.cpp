#include "passwdedit.h"

#include <QtCore/QObject>
#include <QDebug>
#include <QSettings>

PassWdEdit::PassWdEdit(QString iconId, QWidget* parent)
    : QFrame(parent)
{
    m_iconId = iconId;
    initUI();
    initConnect();

    setObjectName("passwdEnterFrame");
    setFixedSize(QSize(270, 36));
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
    connect(m_iconButton, &QPushButton::clicked, this, &PassWdEdit::submit);
}

void PassWdEdit::focusInEvent(QFocusEvent *)
{
    m_lineEdit->setFocus();
}

void PassWdEdit::updateKeybordLayoutStatus(const QString &username) {
    QSettings settings("/var/lib/greeter/users.ini", QSettings::IniFormat);
    // TODO: 这里配置文件是以 ';' 分隔的，但是Qt会把 ';' 之后的认为注释
    m_keyboardList = settings.value(username + "/KeyboardLayoutList").toString().split("_");

    qDebug() << settings.value(username + "/KeyboardLayoutList").toString();
    qDebug() << username << m_keyboardList;

    if (m_keyboardList.count() > 2) {
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
