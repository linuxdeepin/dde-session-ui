/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei_cm@deepin.com>
 *
 * Maintainer: chenwei <chenwei_cm@deepin.com>
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

#include "notifylistview.h"

#include <QWidget>

class Persistence;
class AppGroupModel;
class AppGroupDelegate;
class QLabel;
class QListView;
class NotifyModel;

/*!
 * \~chinese \class NotifyWidget
 * \~chinese \brief 通知中心用来存放所有App组信息的窗口
 */
class NotifyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NotifyWidget(QWidget *parent = nullptr, Persistence *database = nullptr);
    QListView *view() { return m_mainList; }
    NotifyModel *model() { return m_model; }

private:
    void initView(Persistence *database);

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    NotifyListView *m_mainList = nullptr;
    QLabel *m_noNotify = nullptr;
    NotifyModel *m_model;
};

#endif // NOTIFYWIDGET_H
