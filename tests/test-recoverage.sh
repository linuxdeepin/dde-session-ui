#!/bin/bash

BUILD_DIR=build
REPORT_DIR=report

# 工程列表
targets=()
targets[${#targets[*]}]=dde-bluetooth-dialog
targets[${#targets[*]}]=dde-full-filesystem
targets[${#targets[*]}]=dde-license-dialog
targets[${#targets[*]}]=dde-lowpower
targets[${#targets[*]}]=dde-notification-plugin
targets[${#targets[*]}]=dde-osd
targets[${#targets[*]}]=dde-suspend-dialog
targets[${#targets[*]}]=dde-warning-dialog
targets[${#targets[*]}]=dde-welcome
targets[${#targets[*]}]=dde-wm-chooser
targets[${#targets[*]}]=dmemory-warning-dialog
targets[${#targets[*]}]=dnetwork-secret-dialog
# FIXME 编译不通过 targets[${#targets[*]}]=dde-touchscreen-dialog

# 编译源文件
cd ../
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR || return

for target in ${targets[*]}; do
    qmake CONFIG+=debug ../${target}/
    make -j$(nproc)
done

# 编译测试程序
cd ../tests/ || return
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR || return

for target in ${targets[*]}; do
    # 为每个工程创建一个文件夹
    mkdir ${target}
    cd ${target} || continue
    qmake CONFIG+=debug ../../${target}/
    make -j$(nproc)
    # 生成单元测试报告
    cd ..
    cp ${target}/ut_${target} .
    ./ut_${target} --gtest_output=xml:test_report_${target}.xml
    mv asan_${target}.log* asan_${target}.log
done

# 单元测试覆盖率
lcov --directory . --capture --output-file ./coverage.info
lcov --remove ./coverage.info "*/tests/*" "*/usr/include*" "*build/src*" "*persistence.cpp*" "*notifications_dbus_adaptor.cpp*" "*notifysettings.cpp*" "*dbuslogin1manager.cpp*" "*persistence.h*" "*notifysettings.h*" "*dbuslogin1manager.h*" "*dbusdockinterface.h*" "*dbus_daemon_interface.h*" "*dbus_daemon_interface.cpp*" "*/global_util/dbus/*" "*/global_util/xkbparser*" "*icondata.h*" "*icondata.cpp*" "*kblayoutindicator.h*" "*kblayoutindicator.cpp*" -o ./coverage.info

# 生成html
cd .. || return
genhtml -o $REPORT_DIR $BUILD_DIR/coverage.info
