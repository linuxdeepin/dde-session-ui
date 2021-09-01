#!/bin/bash

BUILD_DIR=build
REPORT_DIR=report

# 工程列表
targets=()
targets[${#targets[*]}]=dde-bluetooth-dialog
# TODO targets[${#targets[*]}]=dde-full-filesystem
targets[${#targets[*]}]=dde-license-dialog
targets[${#targets[*]}]=dde-lowpower
# FIXME 影响dock状态 targets[${#targets[*]}]=dde-notification-plugin
targets[${#targets[*]}]=dde-osd
# TODO targets[${#targets[*]}]=dde-pixmix
targets[${#targets[*]}]=dde-suspend-dialog
# TODO targets[${#targets[*]}]=dde-touchscreen-dialog
targets[${#targets[*]}]=dde-warning-dialog
targets[${#targets[*]}]=dde-welcome
targets[${#targets[*]}]=dde-wm-chooser
targets[${#targets[*]}]=dmemory-warning-dialog
targets[${#targets[*]}]=dnetwork-secret-dialog

# 编译源文件
cd ../
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

for target in ${targets[*]} 
do
    qmake CONFIG+=debug ../${target}/
    make -j$(nproc)
done

# 编译测试程序
cd ../tests/
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR

for target in ${targets[*]}
do
    # 为每个工程创建一个文件夹
    mkdir ${target}
    cd ${target}
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
lcov --remove ./coverage.info "*/tests/*" "*/usr/include*" "*build/src*" "*persistence.cpp*" "*notifications_dbus_adaptor.cpp*" "*notifysettings.cpp*" "*dbuslogin1manager.cpp*" "*persistence.h*"  "*notifysettings.h*" "*dbuslogin1manager.h*" "*dbusdockinterface.h*" "*dbus_daemon_interface.h*" "*dbus_daemon_interface.cpp*" "*/global_util/dbus/*" "*icondata.h*" "*icondata.cpp*" "*kblayoutindicator.h*" "*kblayoutindicator.cpp*" -o ./coverage.info

# 生成html
cd ..
genhtml -o $REPORT_DIR $BUILD_DIR/coverage.info
