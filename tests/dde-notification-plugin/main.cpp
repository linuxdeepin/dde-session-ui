// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <gtest/gtest.h>

#include <QApplication>
#include <QDebug>
#include <QTimer>

#ifdef QT_DEBUG
#include <sanitizer/asan_interface.h>
#endif

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "offscreen");
    QApplication app(argc,argv);
    int ret = 0;
    qDebug() << "start dde-notification-plugin test cases ..............";
    ::testing::InitGoogleTest(&argc, argv);
    ret = RUN_ALL_TESTS();
    qDebug() << "end dde-notification-plugin test cases ..............";

#ifdef QT_DEBUG
    __sanitizer_set_report_path("asan_dde-notification-plugin.log");
#endif

    return ret;
}
