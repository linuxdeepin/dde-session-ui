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
    m_lineEdit->installEventFilter(this);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
    m_Layout->addSpacing(3);
    m_Layout->addWidget(m_keyboardButton);
    m_Layout->addSpacing(2);
    m_Layout->addWidget(m_lineEdit);
    m_Layout->addStretch();

    m_opacityEffect = new QGraphicsOpacityEffect;
    m_opacityEffect->setOpacity(1.0);
    m_showAni = new QPropertyAnimation(m_opacityEffect, "opacity");
    m_hideAni = new QPropertyAnimation(m_opacityEffect, "opacity");

    m_iconButton->move(this->x() + this->width()*2 + 14, this->y() - 1);
    setLayout(m_Layout);
    setGraphicsEffect(m_opacityEffect);

    QTimer::singleShot(1000, m_lineEdit, &QLineEdit::grabKeyboard);
}

void PassWdEdit::initConnect() {
    connect(m_iconButton, &QPushButton::clicked, this, &PassWdEdit::submit);
    connect(m_keyboardButton, &QPushButton::clicked, this, &PassWdEdit::keybdLayoutButtonClicked);
    connect(m_hideAni, &QPropertyAnimation::finished, this, &QFrame::hide);
    connect(m_lineEdit, &QLineEdit::textChanged, [this] {
        setAlert(false);
    });
}

void PassWdEdit::initData() {
    utilSettings = new UtilSettings(this);
}

void PassWdEdit::focusInEvent(QFocusEvent *)
{
    m_lineEdit->setFocus();
}

bool PassWdEdit::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_lineEdit && e->type() == QEvent::FocusIn)
        setAlert(false);

    return false;
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

void PassWdEdit::show()
{
    if (isVisible())
        return;

    m_hideAni->stop();
    m_showAni->stop();
    m_showAni->setStartValue(0.0);
    m_showAni->setEndValue(1.0);
    m_showAni->start();

    QFrame::show();
}

void PassWdEdit::hide()
{
    if (!isVisible())
        return;

    m_hideAni->stop();
    m_showAni->stop();
    m_hideAni->setStartValue(1.0);
    m_hideAni->setEndValue(0.0);
    m_hideAni->start();
}

void PassWdEdit::setAlert(bool alert)
{
    if (m_alert == alert)
        return;
    m_alert = alert;
    emit alertChanged(alert);

    if (m_alert) {
        // block text changed signal
        m_lineEdit->blockSignals(true);
        m_lineEdit->setText(tr("Wrong Password"));
        m_lineEdit->blockSignals(false);
        m_lineEdit->setEchoMode(QLineEdit::Normal);
    } else {
        m_lineEdit->clear();
        m_lineEdit->setEchoMode(QLineEdit::Password);
    }

    setStyleSheet(styleSheet());
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
