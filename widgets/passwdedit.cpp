/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore/QObject>
#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QKeyEvent>
#include <QTimer>
#include <QRegExpValidator>
#include "public_func.h"

#include "constants.h"
#include "passwdedit.h"
#include "keyboardmonitor.h"

DWIDGET_USE_NAMESPACE

PassWdEdit::PassWdEdit(QWidget *parent)
    : QFrame(parent),
      m_capslockMonitor(KeyboardMonitor::instance()),
      m_errorTooltip(new ErrorTooltip("", this->parentWidget()))
{
    initUI();
    initConnect();
    initData();
    setObjectName("PassWdEditFrame");
#ifndef SHENWEI_PLATFORM
    setFixedSize(QSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH - 2, 34));
#else
    setFixedSize(QSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH - 1, 34));
#endif

    m_capslockMonitor->start(QThread::LowestPriority);
    connect(m_capslockMonitor, &KeyboardMonitor::capslockStatusChanged,
            this, &PassWdEdit::updateCapslockStatus);
}

PassWdEdit::~PassWdEdit()
{
}

void PassWdEdit::initUI()
{
    m_errorTooltip->hide();

#ifndef SHENWEI_PLATFORM
    m_keyboardButton = new DImageButton;
    m_keyboardButton->setVisible(false);
    m_keyboardButton->setObjectName("KeyBoardLayoutButton");
#endif
    m_iconButton = new DImageButton(this);
//    m_iconButton->setCheckable(true);
    m_iconButton->setFixedSize(QSize(28, 28));

    m_capslockWarning = new QLabel(this);
    m_capslockWarning->setVisible(m_capslockMonitor->isCapslockOn());
    m_capslockWarning->setFixedSize(DDESESSIONCC::CapslockWarningWidth,
                                    DDESESSIONCC::CapslockWarningWidth);
    m_capslockWarning->setPixmap(loadPixmap(":/img/capslock.svg"));

    m_lineEdit = new QLineEdit;
    m_lineEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_lineEdit->setObjectName("passwdLineEdit");
    m_lineEdit->setEchoMode(QLineEdit::Password);
    m_lineEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    m_lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_lineEdit->setAlignment(Qt::AlignVCenter);
    m_lineEdit->installEventFilter(this);

    QFont font = m_lineEdit->font();
    font.setWeight(QFont::Normal);
    m_lineEdit->setFont(font);

    m_Layout = new QHBoxLayout;
    m_Layout->setMargin(0);
    m_Layout->setSpacing(0);
#ifndef SHENWEI_PLATFORM
    m_Layout->addWidget(m_keyboardButton, 0, Qt::AlignVCenter);
#endif
    m_Layout->addSpacing(4);
    m_Layout->addWidget(m_lineEdit, 1, Qt::AlignVCenter);
    m_Layout->addSpacing(2);
    m_Layout->addWidget(m_capslockWarning, 0, Qt::AlignVCenter);
    m_Layout->addSpacing(2);
    m_Layout->addWidget(m_iconButton, 0, Qt::AlignVCenter);
    m_Layout->addSpacing(3);

    m_opacityEffect = new QGraphicsOpacityEffect;
    m_opacityEffect->setOpacity(1.0);
    m_showAni = new QPropertyAnimation(m_opacityEffect, "opacity");
    m_hideAni = new QPropertyAnimation(m_opacityEffect, "opacity");

    setLayout(m_Layout);
    setGraphicsEffect(m_opacityEffect);

    getFocusTimer = new QTimer(this);
    getFocusTimer->setInterval(100);
    connect(getFocusTimer,  &QTimer::timeout, this, &PassWdEdit::lineEditGrabKeyboard);
#ifndef SHENWEI_PLATFORM
    updateStyle(":/skin/passwdedit.qss", this);
#endif

}

void PassWdEdit::lineEditGrabKeyboard()
{
    //    qDebug() << "lineEditGrabKeyboard" << m_timerCount;
    if (m_timerCount == 10) {
        getFocusTimer->stop();
        m_timerCount = 0;
    } else {
        m_timerCount++;
        m_lineEdit->grabKeyboard();
    }
}

void PassWdEdit::recordUserPassWd(bool isChoose, QString username)
{
    qDebug() << "remember username:" << username;
    if (isChoose && !m_lineEdit->text().isEmpty()) {
        passwordMap.insert(username, m_lineEdit->text());
        qDebug() << "m_lineEdit:" << m_lineEdit->text();
        m_lineEdit->setText("");
        QMap<QString, QString>::const_iterator i = passwordMap.begin();
        while (i != passwordMap.end()) {
            qDebug() << "record the score!" << passwordMap.count()
                     << i.key() << i.value();
            ++i;
        }
    } else {
        qDebug() << "select currentuser:" << username;
        QMap<QString, QString>::const_iterator i = passwordMap.begin();
        bool ownsPasswd = false;
        while (i != passwordMap.end()) {
            if (i.key() == username) {
                ownsPasswd = true;
                qDebug() << "get the score!";
                m_lineEdit->setText(i.value());
                break;
            }
            ++i;
        }
        if (!ownsPasswd) {
            m_lineEdit->setText("");
        }
    }
}

void PassWdEdit::updateCapslockStatus(bool on)
{
    m_capslockWarning->setVisible(on);
}

void PassWdEdit::selectAll() const
{
    m_lineEdit->selectAll();
}

void PassWdEdit::initConnect()
{
    connect(m_iconButton, &DImageButton::clicked, this, &PassWdEdit::submit);
#ifndef SHENWEI_PLATFORM
    connect(m_keyboardButton, &DImageButton::clicked, this, &PassWdEdit::keybdLayoutButtonClicked);
#endif
    connect(m_hideAni, &QPropertyAnimation::finished, this, &QFrame::hide);
    connect(m_lineEdit, &QLineEdit::textChanged, [this] {
        setAlert(false);
    });
}

void PassWdEdit::initData()
{
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
    if (!isEnabled()) return false;

    if (o == m_lineEdit && (e->type() == QEvent::MouseButtonRelease ||
                            e->type() == QEvent::KeyRelease)) {

        if (m_lineEdit->isReadOnly()) {
            m_lineEdit->setReadOnly(false);
            setAlert(false);
            m_lineEdit->setFocusPolicy(Qt::StrongFocus);

            if (e->type() == QEvent::KeyRelease) {
                QKeyEvent *event = static_cast<QKeyEvent *>(e);
                qDebug() << "passwdedit:" << event->text();
                if (event->text().length() == 1 && event->key() != Qt::Key_Escape &&
                        event->key() != Qt::Key_Tab) {
                    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
                        m_lineEdit->setText("");
                        m_alert_enter = true;
                    } else if (event->key() == Qt::Key_Backspace) {
                        m_lineEdit->setText("");
                        m_alert_enter = m_alert_enter ? !m_alert_enter : m_alert_enter;
                    } else {
                        m_errorTooltip->hide();
                        m_lineEdit->setText(event->text());
                        m_alert_enter = m_alert_enter ? !m_alert_enter : m_alert_enter;
                    }
                    qDebug() << "m_lineEdit:" << m_lineEdit->text() << m_lineEdit->cursorPosition();
                }
            }
        }
    }
    if (o == m_lineEdit && e->type() == QEvent::FocusIn) {
        setAlert(false);
    }

    if (o == m_lineEdit && e->type() == QEvent::Show) {
        getFocusTimer->start();
    }

    return false;
}

#ifndef SHENWEI_PLATFORM
void PassWdEdit::updateKeybdLayoutUI(QStringList keybdList)
{
    if (keybdList.count() > 1) {
        m_keyboardButton->show();
    } else {
        m_keyboardButton->hide();
    }
}
#endif

void PassWdEdit::show()
{
    if (isVisible()) {
        return;
    }

    m_hideAni->stop();
    m_showAni->stop();
    m_showAni->setStartValue(0.0);
    m_showAni->setEndValue(1.0);
    m_showAni->start();

    QFrame::show();
}

void PassWdEdit::hide()
{
    if (!isVisible()) {
        return;
    }

    m_hideAni->stop();
    m_showAni->stop();
    m_hideAni->setStartValue(1.0);
    m_hideAni->setEndValue(0.0);
    m_hideAni->start();
    m_errorTooltip->hide();
}

bool PassWdEdit::isReadOnly()
{
    return m_lineEdit->isReadOnly();
}

void PassWdEdit::setReadOnly(bool value)
{
    m_lineEdit->setReadOnly(value);
}

void PassWdEdit::setAlert(bool alert, const QString &text)
{
    if (m_alert == alert) {
        return;
    }

    m_alert = alert;
    emit alertChanged(alert);

    if (m_alert) {
        // block text changed signal
        const QPoint pos = mapTo(m_errorTooltip->parentWidget(), QPoint(m_lineEdit->x(), height() - 10));
        m_errorTooltip->setMessage(text);
        m_errorTooltip->show(pos.x(), pos.y());
    } else {
        m_errorTooltip->hide();
    }

    setStyleSheet(styleSheet());
}

void PassWdEdit::setMessage(const QString &message)
{
    m_lineEdit->setText("");
    m_lineEdit->setPlaceholderText(message);
}

void PassWdEdit::keyReleaseEvent(QKeyEvent *e)
{
    emit focusIn();

    //    qDebug() << "PassWordEdit e->key:" << e->key();
    switch (e->key()) {
    case Qt::Key_Return:        /* submit */
    case Qt::Key_Enter:
        if (m_alert_enter) {
            m_alert_enter = false;
        } else {
            emit submit();
        }
        break;
    case Qt::Key_Left:          emit leftKeyPressed();      break;
    case Qt::Key_Right:         emit rightKeyPressed();     break;
    default:;
    }
}

QString PassWdEdit::getText()
{
    return m_lineEdit->text();
}

#ifdef LOCK_NO_QUIT
void PassWdEdit::clearText() {
    m_lineEdit->clear();
}
#endif

void PassWdEdit::setEnterBtnStyle(const QString &normalPic, const QString &hoverPic, const QString &pressedPic)
{
    m_iconButton->setNormalPic(normalPic);
    m_iconButton->setHoverPic(hoverPic);
    m_iconButton->setPressPic(pressedPic);
}
