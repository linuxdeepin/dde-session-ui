#include <QtCore/QObject>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QKeyEvent>
#include <QTimer>

#include "constants.h"
#include "passwdedit.h"

PassWdEdit::PassWdEdit(QWidget* parent)
    : QFrame(parent)
{
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

    m_keyboardButton->setFixedSize(QSize(20, 14));
    m_keyboardButton->setIconSize(QSize(20, 14));
    m_iconButton = new DImageButton(this);
    m_iconButton->setCheckable(true);
    m_iconButton->setFixedSize(QSize(35, 35));

    m_lineEdit = new QLineEdit;
    m_lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
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

    m_iconButton->move(this->x() + this->width()*2 + 13, this->y());
    setLayout(m_Layout);
    setGraphicsEffect(m_opacityEffect);

    QTimer::singleShot(1000, this, SLOT(lineEditGrabKeyboard()));

    updateStyle(":/skin/passwdedit.qss", this);

}

void PassWdEdit::lineEditGrabKeyboard() {
    m_lineEdit->grabKeyboard();
}
void PassWdEdit::initConnect() {
    connect(m_iconButton, &DImageButton::clicked, this, &PassWdEdit::submit);
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
    if (m_lineEdit->isReadOnly()) {
        m_lineEdit->setReadOnly(false);
    }
}

bool PassWdEdit::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_lineEdit && (e->type() == QEvent::MouseButtonRelease ||
         e->type() == QEvent::KeyRelease)) {

        if (m_lineEdit->isReadOnly()) {
            m_lineEdit->setReadOnly(false);
            setAlert(false);
            m_lineEdit->setFocusPolicy(Qt::StrongFocus);

            if (e->type() == QEvent::KeyRelease) {
                QKeyEvent *event = static_cast<QKeyEvent*>(e);

                if (event->text().length()==1 && event->key()!=Qt::Key_Escape &&
                        event->key() != Qt::Key_Tab) {
                    m_lineEdit->setText(event->text());

                    qDebug() << "m_lineEdit:" << m_lineEdit->text() << m_lineEdit->cursorPosition();
                }

            }
        }
    }
    if (o == m_lineEdit && e->type() == QEvent::FocusIn) {
        setAlert(false);
    }

    return false;
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

void PassWdEdit::setAlert(bool alert, const QString &text)
{
    if (m_alert == alert)
        return;
    m_alert = alert;
    emit alertChanged(alert);

    if (m_alert) {
        // block text changed signal
        m_lineEdit->blockSignals(true);
        m_lineEdit->setText(text);
        m_lineEdit->blockSignals(false);
        m_lineEdit->setEchoMode(QLineEdit::Normal);
        m_lineEdit->setReadOnly(true);
    } else {
        m_lineEdit->clear();
        m_lineEdit->setEchoMode(QLineEdit::Password);
        m_lineEdit->setReadOnly(false);
    }

    setStyleSheet(styleSheet());
}

void PassWdEdit::keyReleaseEvent(QKeyEvent *e)
{
    emit focusIn();
    qDebug() << "e->key:" << e->key();
    switch (e->key())
    {
    case Qt::Key_Return:        /* submit */
    case Qt::Key_Enter:         emit submit();              break;
    case Qt::Key_Left:          emit leftKeyPressed();      break;
    case Qt::Key_Right:         emit rightKeyPressed();     break;
    default:;
    }
}

QString PassWdEdit::getText() {
    return m_lineEdit->text();
}

void PassWdEdit::setEnterBtnStyle(const QString &normalPic, const QString &hoverPic, const QString &pressedPic)
{
    m_iconButton->setNormalPic(normalPic);
    m_iconButton->setHoverPic(hoverPic);
    m_iconButton->setPressPic(pressedPic);
}
