/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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

static const int SessionButtonWidth = 160;
static const int SessionButtonHeight = 160;

const QString session_standard_icon_name(const QString &realName)
{
    const QStringList standard_icon_list = {
        "deepin",
        "fluxbox",
        "gnome",
        "kde",
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
      m_userSettings(DDESESSIONCC::CONFIG_FILE, QSettings::IniFormat),
      m_sessionModel(new QLightDM::SessionsModel(this))
{
    setStyleSheet("QFrame {"
                  "background-color: red;"
                  "}");

    loadSessionList();
}

SessionWidget::~SessionWidget()
{
    qDeleteAll(m_sessionBtns);
}

const QString SessionWidget::currentSessionName() const
{
    return m_sessionModel->data(m_sessionModel->index(m_currentSessionIndex), Qt::DisplayRole).toString();
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
    return m_userSettings.value(QString("%1/last-session").arg(m_currentUser)).toString();
}

void SessionWidget::saveSettings()
{
    qDebug() << "save user session: " << m_currentUser << currentSessionName();

    m_userSettings.setValue(QString("%1/last-session").arg(m_currentUser), currentSessionName());
}

void SessionWidget::switchToUser(const QString &userName)
{
    if (m_currentUser == userName)
        return;

    m_currentUser = userName;
    const QString sessionName = lastSessionName();

    qDebug() << userName << "default session is: " << sessionName << m_currentSessionIndex;
}

void SessionWidget::switchSession(const QString &sessionName)
{
    qDebug() << "switch to " << sessionName;
    m_currentSessionIndex = sessionIndex(sessionName);
    m_sessionBtns.at(m_currentSessionIndex)->setChecked(true);

    // TODO: emit after hide animation finished
    emit sessionChanged(sessionName);
}

int SessionWidget::sessionIndex(const QString &sessionName)
{
    const int count = m_sessionModel->rowCount(QModelIndex());
    for (int i(0); i != count; ++i)
        if (m_sessionModel->data(m_sessionModel->index(i), Qt::DisplayRole) == sessionName)
            return i;

    Q_UNREACHABLE();
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
        const QString &session_name = m_sessionModel->data(m_sessionModel->index(i), Qt::DisplayRole).toString();
        const QString &session_icon = session_standard_icon_name(session_name);
        const QString normalIcon = QString(":/img/sessions_icon/%1_normal.png").arg(session_icon);
        const QString hoverIcon = QString(":/img/sessions_icon/%1_hover.png").arg(session_icon);
        const QString checkedIcon = QString(":/img/sessions_icon/%1_press.png").arg(session_icon);

        qDebug() << "found session: " << session_name << session_icon;
        RoundItemButton *sbtn = new RoundItemButton(session_name, this);
        sbtn->setFixedSize(SessionButtonWidth, SessionButtonHeight);
        sbtn->setAutoExclusive(true);
        sbtn->setProperty("normalIcon", normalIcon);
        sbtn->setProperty("hoverIcon", hoverIcon);
        sbtn->setProperty("checkedIcon", checkedIcon);
        sbtn->hide();

        m_sessionBtns.append(sbtn);
    }
}
