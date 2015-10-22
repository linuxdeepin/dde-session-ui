#include <QtCore/QObject>
#include <QDebug>
#include <QSettings>
#include <QFile>

#include "passwdedit.h"

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
    connect(m_keyboardButton, &QPushButton::clicked, this, &PassWdEdit::keybdLayoutButtonClicked);

}

void PassWdEdit::focusInEvent(QFocusEvent *)
{
    m_lineEdit->setFocus();
}

void PassWdEdit::updateKeybordLayoutStatus(const QString &username) {

    QSettings settings("/var/lib/greeter/users.ini", QSettings::IniFormat);
    // TODO: 这里配置文件是以 ';' 分隔的，但是Qt会把 ';' 之后的认为注释
    m_keyboardList = settings.value(username + "/KeyboardLayoutList").toString().split("_");
    getCurrentKeyboardLayout(username);
    qDebug() << "m_keyboardList:" << m_keyboardList;

    if (m_keyboardList.count() > 2) {
        m_keyboardButton->show();
        setFixedWidth(270);
    } else {
        m_keyboardButton->hide();
        setFixedWidth(240);
    }
    emit updateKeyboardStatus();
}

void PassWdEdit::getCurrentKeyboardLayout(QString username) {

    QSettings settings("/var/lib/greeter/users.ini", QSettings::IniFormat);
    m_keyboardList = settings.value(username + "/KeyboardLayoutList").toString().split(" ");
//    qDebug() << username << m_keyboardList;

    for (int i = 0; i < m_keyboardList.length(); i++) {
        QStringList itemKeyboardList = m_keyboardList[i].split("|");
//        qDebug() << "itemKeyboardList:" << itemKeyboardList;
        keybdLayoutMap.insert(itemKeyboardList[0], itemKeyboardList[1]);
    }
//    qDebug() << "keybdLayoutMap:" << keybdLayoutMap;

    m_parseMainDescriptionXml = new ParseXML("/usr/share/X11/xkb/rules/base.xml", "configItem", "name", "description", this);
    QMap<QString, QString> tmpMainMap = m_parseMainDescriptionXml->getTagNodeInfo();
//    qDebug() << "MainMap:" << tmpMainMap;
    QMap<QString, QString>::iterator i ;

    for(i = keybdLayoutMap.begin(); i != keybdLayoutMap.end(); i++) {
        if (!tmpMainMap.value(i.key()).isEmpty()) {
            keybdLayoutMainDescriptionMap.insert(i.key(), tmpMainMap.value(i.key()));
        }
    }
    qDebug() << "MAMAMAMAMA keyboardLayout:" << keybdLayoutMainDescriptionMap;

}
void PassWdEdit::setLineEditRightImage(QString imageUrl) {
    m_iconButton->setIcon(QIcon(QPixmap(imageUrl)));
}

QString PassWdEdit::getText() {
    return m_lineEdit->text();
}
