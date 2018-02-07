/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#include "sessionwidget.h"
#include "constants.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QLabel>
#include <QFile>
#include <QButtonGroup>
#include <QSettings>
#include <QPropertyAnimation>
#include <QString>

static const int SessionButtonWidth = 160;
static const int SessionButtonHeight = 160;

const QString session_standard_icon_name(const QString &realName)
{
    const QStringList standard_icon_list = {
        "deepin",
        "fluxbox",
        "gnome",
        "plasma",
        "ubuntu",
        "xfce"
    };

    for (const auto &name : standard_icon_list)
        if (realName.contains(name, Qt::CaseInsensitive))
            return name;

    return QStringLiteral("unknow");
}

SessionWidget::SessionWidget(QWidget *parent)
    : QFrame(parent),
      m_currentSessionIndex(0),
      m_sessionModel(new QLightDM::SessionsModel(this))
{
//    setStyleSheet("QFrame {"
//                  "background-color: red;"
//                  "}");

    loadSessionList();
}

SessionWidget::~SessionWidget()
{
    qDeleteAll(m_sessionBtns);
}

const QString SessionWidget::currentSessionName() const
{
    return m_sessionModel->data(m_sessionModel->index(m_currentSessionIndex), QLightDM::SessionsModel::KeyRole).toString();
}

const QString SessionWidget::currentSessionKey() const
{
    return m_sessionModel->data(m_sessionModel->index(m_currentSessionIndex), QLightDM::SessionsModel::KeyRole).toString();
}

void SessionWidget::show()
{
    if (isVisible())
        return;

    const int itemPadding = 20;
    const int itemWidth = m_sessionBtns.first()->width();
    const int itemHeight = m_sessionBtns.first()->height();
    const int itemTotal = itemPadding + itemWidth;
    const int count = m_sessionBtns.count();
    const int maxLineCap = width() / itemTotal - 1; // 1 for left-offset and right-offset.
    const int offset = (width() - itemTotal * qMin(count, maxLineCap)) / 2;

    // Adjust size according to session count.
    if (maxLineCap < count) {
        setFixedSize(width(), qCeil(count * 1.0 / maxLineCap) * SessionButtonHeight);
    }

    for (int i(0); i != count; ++i) {
        QPropertyAnimation *ani = new QPropertyAnimation(m_sessionBtns[i], "pos");
        if (i + 1 <= maxLineCap) {
            // the first line.
            ani->setStartValue(QPoint(width(), 0));
            ani->setEndValue(QPoint(offset + i * itemTotal, 0));
        } else {
            // the second line.
            ani->setStartValue(QPoint(width(), itemHeight));
            ani->setEndValue(QPoint(offset + (i - maxLineCap) * itemTotal, itemHeight));
        }
        ani->start(QAbstractAnimation::DeleteWhenStopped);

        m_sessionBtns.at(i)->show();
    }

    // checked default session button
    m_sessionBtns.at(m_currentSessionIndex)->setChecked(true);

    QWidget::show();
}

int SessionWidget::sessionCount() const
{
    return m_sessionModel->rowCount(QModelIndex());
}

const QString SessionWidget::lastSessionName() const
{
    QSettings setting(DDESESSIONCC::CONFIG_FILE + m_currentUser, QSettings::IniFormat);
    setting.beginGroup("User");
    const QString &session = setting.value("XSession").toString();

    return session.isEmpty() ? m_sessionModel->data(m_sessionModel->index(0), QLightDM::SessionsModel::KeyRole).toString() : session;
}

void SessionWidget::switchToUser(const QString &userName)
{
    qDebug() << "switch to user" << userName;
    if (m_currentUser != userName)
        m_currentUser = userName;

    const QString sessionName = lastSessionName();
    m_currentSessionIndex = sessionIndex(sessionName);

    qDebug() << userName << "default session is: " << sessionName << m_currentSessionIndex;

    emit sessionChanged(currentSessionName());
}

void SessionWidget::onSessionButtonClicked()
{
    RoundItemButton *btn = qobject_cast<RoundItemButton *>(sender());
    Q_ASSERT(btn);
    Q_ASSERT(m_sessionBtns.contains(btn));

    btn->setChecked(true);
    m_currentSessionIndex = m_sessionBtns.indexOf(btn);

    emit sessionChanged(currentSessionName());
}

int SessionWidget::sessionIndex(const QString &sessionName)
{
    const int count = m_sessionModel->rowCount(QModelIndex());
    Q_ASSERT(count);
    for (int i(0); i != count; ++i)
        if (!sessionName.compare(m_sessionModel->data(m_sessionModel->index(i), QLightDM::SessionsModel::KeyRole).toString(), Qt::CaseInsensitive))
            return i;

    // NOTE: The current session does not exist
    qWarning() << "The session does not exist, using the default value.";
    return 0;
}

//void SessionWidget::leftKeySwitch() {
//    m_sessionBtns.at(m_currentSessionIndex)->updateState(RoundItemButton::Normal);
//    if (m_currentSessionIndex == 0) {
//        m_currentSessionIndex = m_sessionModel->rowCount(QModelIndex()) - 1;
//    } else {
//        m_currentSessionIndex  -= 1;
//    }
//    m_sessionBtns.at(m_currentSessionIndex)->updateState(RoundItemButton::Checked);
//}

//void SessionWidget::rightKeySwitch() {
//    m_sessionBtns.at(m_currentSessionIndex)->updateState(RoundItemButton::Normal);
//    if (m_currentSessionIndex == m_sessionModel->rowCount(QModelIndex()) - 1) {
//        m_currentSessionIndex = 0;
//    } else {
//        m_currentSessionIndex += 1;
//    }
//    m_sessionBtns.at(m_currentSessionIndex)->updateState(RoundItemButton::Checked);
//}

//void SessionWidget::chooseSession() {
//    emit m_sessionBtns.at(m_currentSessionIndex)->clicked();
//}

void SessionWidget::loadSessionList()
{
    // add sessions button
    const int count = m_sessionModel->rowCount(QModelIndex());
    for (int i(0); i != count; ++i)
    {
        const QString &session_name = m_sessionModel->data(m_sessionModel->index(i), QLightDM::SessionsModel::KeyRole).toString();
        const QString &session_icon = session_standard_icon_name(session_name);
        const QString normalIcon = QString(":/img/sessions_icon/%1_normal.svg").arg(session_icon);
        const QString hoverIcon = QString(":/img/sessions_icon/%1_hover.svg").arg(session_icon);
        const QString checkedIcon = QString(":/img/sessions_icon/%1_press.svg").arg(session_icon);

        qDebug() << "found session: " << session_name << session_icon;
        RoundItemButton *sbtn = new RoundItemButton(session_name, this);
        sbtn->setFixedSize(SessionButtonWidth, SessionButtonHeight);
        sbtn->setAutoExclusive(true);
        sbtn->setProperty("normalIcon", normalIcon);
        sbtn->setProperty("hoverIcon", hoverIcon);
        sbtn->setProperty("checkedIcon", checkedIcon);
        sbtn->hide();

        connect(sbtn, &RoundItemButton::clicked, this, &SessionWidget::onSessionButtonClicked);

        m_sessionBtns.append(sbtn);
    }
}
