/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef MULTIUSERSWARNINGVIEW_H
#define MULTIUSERSWARNINGVIEW_H

#include <QFrame>
#include "warningview.h"
#include <dimagebutton.h>
#include "common.h"
#include "rounditembutton.h"
#include "userinfo.h"

class QListWidget;
class QLabel;
class QVBoxLayout;

DWIDGET_USE_NAMESPACE

class MultiUsersWarningView : public WarningView
{
    Q_OBJECT
public:
    MultiUsersWarningView(QWidget *parent = 0);
    ~MultiUsersWarningView();

    void setUsers(QList<std::shared_ptr<User>> users);

    Actions action() const;
    void setAction(const Actions action);

    void toggleButtonState() Q_DECL_OVERRIDE;
    void buttonClickHandle() Q_DECL_OVERRIDE;
    void setAcceptReason(const QString &reason) Q_DECL_OVERRIDE;

signals:
    void actionInvoked();
    void cancelled();

private:
    QString getUserIcon(const QString &path);

private:
    QVBoxLayout * m_vLayout;
    QListWidget * m_userList;
    QLabel * m_warningTip;
    RoundItemButton * m_cancelBtn;
    RoundItemButton * m_actionBtn;
    RoundItemButton *m_currentBtn;
    Actions m_action;
};

class UserListItem : public QFrame
{
    Q_OBJECT
public:
    UserListItem(const QString &icon, const QString &name);

private:
    QLabel * m_icon;
    QLabel * m_name;

    QPixmap getRoundPixmap(const QString &path);
};

#endif // MULTIUSERSWARNINGVIEW_H
