// SPDX-FileCopyrightText: 2018 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
