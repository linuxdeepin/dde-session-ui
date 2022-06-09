/*
 * Copyright (C) 2018 ~ 2028 Uniontech Technology Co., Ltd.
 *
 * Author:     chenwei <chenwei@uniontech.com>
 *
 * Maintainer: chenwei <chenwei@uniontech.com>
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
#ifndef UNITTEST_H
#define UNITTEST_H

#include "notification/constants.h"

#include <QtTest>
#include <com_deepin_dde_notification.h>

using NotifyInter = com::deepin::dde::Notification;

class UnitTest : public QObject
{
    Q_OBJECT

public:
    UnitTest();
    ~UnitTest();

private slots:
    EntityPtr createEntity();
    void test_NotifyInter();    // 测试通知中心接口
    void test_BubbleTool();     // 测试BubbleTool类
    void test_BubbleItem();     // 测试BubbleItem类
    void test_NotifyModel();    // 测试NotifyModel类
    void test_Bubble();         // 测试Bubble类
    void test_Persistence();    // 测试Persistence类

private:
    NotifyInter *m_notifyInter;
};

#endif // UNITTEST_H
