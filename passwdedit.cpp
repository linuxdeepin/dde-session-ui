#include <QtCore/QObject>
#include "passwdedit.h"

PassWdEdit::PassWdEdit(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("passwdEnterFrame");
    m_lockLabel = new QLabel;
    m_lockLabel->setFixedSize(QSize(36,36));
    switchStatus(true);

    m_lineEdit = new QLineEdit;
    m_lineEdit->setObjectName("passwdLineEdit");
    switchStatus(true);
    m_lineEdit->setEchoMode(QLineEdit::Password);
    m_lineEdit->setFixedSize(QSize(200, m_lockLabel->height()));
    setFixedSize(QSize(236, 36));
    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addWidget(m_lineEdit);
    m_Layout->addWidget(m_lockLabel);

    setLayout(m_Layout);
}
void PassWdEdit::switchStatus(bool isLogin) {
    if (isLogin) {
        setRightIcon(":img/login_normal.png");
    } else {
        setRightIcon(":img/passwdlock.png");
    }
}
void PassWdEdit::setRightIcon(QString iconUrl) {
    m_lockLabel->setPixmap(QPixmap(iconUrl));
}
PassWdEdit::~PassWdEdit()
{

}
