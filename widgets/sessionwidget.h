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

#ifndef SESSIONWIDGET_H
#define SESSIONWIDGET_H

#include <QFrame>
#include <QList>
#include <QSettings>

#include <QLightDM/SessionsModel>

#include "rounditembutton.h"

class SessionWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SessionWidget(QWidget *parent = 0);
    ~SessionWidget();

    void show();
    int sessionCount() const;
    const QString lastSessionName() const;
    const QString currentSessionName() const;
    const QString currentSessionKey() const;
    const QString currentSessionOwner() const { return m_currentUser; }

signals:
    void sessionChanged(const QString &sessionName);

public slots:
    void switchToUser(const QString &userName);
    //    void leftKeySwitch();
    //    void rightKeySwitch();
    //    void chooseSession();

private slots:
    void loadSessionList();

private slots:
    void onSessionButtonClicked();

private:
    int sessionIndex(const QString &sessionName);

private:
    int m_currentSessionIndex;
    QString m_currentUser;

    QLightDM::SessionsModel *m_sessionModel;
    QList<RoundItemButton *> m_sessionBtns;
};

#endif // SESSIONWIDGET_H
