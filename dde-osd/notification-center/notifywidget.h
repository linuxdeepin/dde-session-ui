/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <190771752ri@gmail.com>
 *
 * Maintainer: listenerri <190771752ri@gmail.com>
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

#ifndef NOTIFYWIDGET_H
#define NOTIFYWIDGET_H

#include <QWidget>
#include <QListView>

class Persistence;
class AppGroupModel;
class AppGroupDelegate;
class QLabel;

class NotifyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotifyWidget(QWidget *parent = nullptr, Persistence *database = nullptr);

private:
    void initView(Persistence *database);

private:
    AppGroupDelegate *m_groupDelegate = nullptr;
    AppGroupModel *m_groupModel = nullptr;
    QListView *m_notifyView = nullptr;
    QLabel *m_noNotify = nullptr;
};

#endif // NOTIFYWIDGET_H
