#include <QtCore/QObject>
#include <QDebug>
#include "passwdedit.h"

PassWdEdit::PassWdEdit(QString iconId, QWidget* parent)
    : QFrame(parent)
{
    m_iconId = iconId;
    initUI();
    initConnect();
}
void PassWdEdit::initUI() {
    setObjectName("passwdEnterFrame");
    setFixedSize(QSize(270, 36));

    m_keyboardButton = new QPushButton;
    m_keyboardButton->setObjectName("KeyBoardLayoutButton");
    m_keyboardButton->setFixedSize(QSize(30, 24));
    //m_keyboardButton->setIconSize(QSize(24, 24));
    //m_keyboardButton->setIcon(QIcon(QPixmap(":img/keyboard.png")));
    m_iconButton = new QPushButton;
    m_iconButton->setObjectName(m_iconId);
    m_iconButton->setCheckable(true);
    m_iconButton->setFixedSize(QSize(36,36));
    m_iconButton->setIconSize(QSize(36, 36));
    m_iconButton->sizeHint();
//    setLineEditRightImage(iconId);

    m_lineEdit = new QLineEdit;
    m_lineEdit->setObjectName("passwdLineEdit");
    m_lineEdit->setEchoMode(QLineEdit::Password);
    m_lineEdit->setFixedSize(QSize(200, m_iconButton->height()));



    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addWidget(m_keyboardButton);
    m_Layout->addWidget(m_lineEdit);
    m_Layout->addWidget(m_iconButton);
    setLayout(m_Layout);

    setKeyBoardLayoutButtonStatus(false);
}
void PassWdEdit::initConnect() {
    connect(m_iconButton, SIGNAL(clicked()), this, SIGNAL(jumpTo()));
    connect(this, SIGNAL(jumpTo()), this, SLOT(testing()));
//    connect(m_iconButton, SIGNAL(clicked()), this, SLOT(testing()));
}
void PassWdEdit::testing() {
    qDebug() << "testing:";
}
void PassWdEdit::setKeyBoardLayoutButtonStatus(bool show) {
    if (show) {
        m_keyboardButton->show();
        setFixedWidth(270);
    } else {
        setFixedWidth(240);
        m_keyboardButton->hide();
    }
}
void PassWdEdit::setLineEditRightImage(QString imageUrl) {
    m_iconButton->setIcon(QIcon(QPixmap(imageUrl)));
}
QString PassWdEdit::getText() {
    return m_lineEdit->text();
}
PassWdEdit::~PassWdEdit()
{
}
