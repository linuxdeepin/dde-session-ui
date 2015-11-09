#include <QtCore/QObject>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QKeyEvent>
#include <QTimer>

#include "constants.h"
#include "passwdedit.h"

PassWdEdit::PassWdEdit(QString iconId, QWidget* parent)
    : QFrame(parent)
{
    m_iconId = iconId;
    initUI();
    initConnect();
    initData();
    setObjectName("PassWdEditFrame");
    setFixedSize(QSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH - 2, 34));
}

PassWdEdit::~PassWdEdit()
{
}

void PassWdEdit::initUI() {
    m_keyboardButton = new QPushButton;
    m_keyboardButton->setObjectName("KeyBoardLayoutButton");

    m_keyboardButton->setFixedSize(QSize(24, 24));
    m_iconButton = new QPushButton(this);
    m_iconButton->setObjectName(m_iconId);
    m_iconButton->setCheckable(true);
    m_iconButton->setFixedSize(QSize(35, 35)); // 34 = 36 - borderTop - borderBottom
    m_iconButton->setIconSize(QSize(35, 35));

    m_lineEdit = new QLineEdit;
    m_lineEdit->setObjectName("passwdLineEdit");
    m_lineEdit->setEchoMode(QLineEdit::Password);
    m_lineEdit->setFixedSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH - m_iconButton->width()
                             - m_keyboardButton->width() - 2,m_iconButton->height());
    m_lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addSpacing(3);
    m_Layout->addWidget(m_keyboardButton);
    m_Layout->addSpacing(2);
    m_Layout->addWidget(m_lineEdit);
    m_Layout->addStretch();

    m_iconButton->move(this->x() + this->width()*2 + 14, this->y() - 1);
    setLayout(m_Layout);

    QTimer::singleShot(1000, m_lineEdit, &QLineEdit::grabKeyboard);
}

void PassWdEdit::initConnect() {
    connect(m_iconButton, &QPushButton::clicked, this, &PassWdEdit::submit);
    connect(m_keyboardButton, &QPushButton::clicked, this, &PassWdEdit::keybdLayoutButtonClicked);

}

void PassWdEdit::initData() {
    utilSettings = new UtilSettings(this);
}

void PassWdEdit::focusInEvent(QFocusEvent *)
{
    m_lineEdit->setFocus();
}

void PassWdEdit::initInsideFrame() {
    m_insideFrame = new QFrame(this);
    m_insideFrame->raise();

    m_insideFrame->setObjectName("PswdInsideFrame");
    QHBoxLayout *insideLayout = new QHBoxLayout(this);
    insideLayout->setContentsMargins(0, 0, 0, 0);
    insideLayout->setSpacing(0);
    insideLayout->addWidget(m_insideFrame);
}

void PassWdEdit::updateKeybordLayoutStatus(const QString &username)
{

    keyboardLayoutList = utilSettings->getKbdLayoutList(username);
    if (keyboardLayoutList.isEmpty()) {
        keyboardLayoutList << "";
    }
    qDebug() << "get UpdateKeybordLayoutStatus";

    if (keyboardLayoutList.count() > 2) {
        m_keyboardButton->show();
    } else {
        m_keyboardButton->hide();
    }
    emit updateKeyboardStatus();
}

void PassWdEdit::updateKeybdLayoutUI(QStringList keybdList) {
    if (keybdList.count() > 2) {
        m_keyboardButton->show();
    } else {
        m_keyboardButton->hide();
    }
}

void PassWdEdit::keyReleaseEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Return:        /* submit */
    case Qt::Key_Enter:         emit submit();              break;
    default:;
    }
}

void PassWdEdit::setLineEditRightImage(QString imageUrl) {
    m_iconButton->setIcon(QIcon(QPixmap(imageUrl)));
}

QString PassWdEdit::getText() {
    return m_lineEdit->text();
}
