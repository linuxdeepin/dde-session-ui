#!/bin/bash

BUILD_DIR=build
REPORT_DIR=report
#EXTRACT_ARGS="src"

cd ../
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
qmake ../dde-osd/
make
#qmake ../dde-license-dialog/
#make
#qmake ../dde-switchtogreeter/
#make
#qmake ../dde-lowpower/
#make
#qmake ../dde-suspend-dialog/
#make
#qmake ../dde-warning-dialog/
#make
#qmake ../dde-welcome/
#make
#qmake ../dde-wm-chooser/
#make
#qmake ../dmemory-warning-dialog/
#make
#qmake ../dnetwork-secret-dialog/
#make
#qmake ../dde-full-filesystem/
#make
#qmake ../dde-bluetooth-dialog/
#make
#qmake ../dde-notification-plugin/
#make
#qmake ../dde-pixmix/
#make
#qmake ../dde-touchscreen-dialog/
#make

cd ../tests/
rm -rf $BUILD_DIR
mkdir $BUILD_DIR
cd $BUILD_DIR
qmake ../dde-osd/
make check
#qmake ../dde-license-dialog/
#make check
#qmake ../dde-switchtogreeter/
#make check
#qmake ../dde-lowpower/
#make check
#qmake ../dde-suspend-dialog/
#make check
#qmake ../dde-warning-dialog/
#make check
#qmake ../dde-welcome/
#make check
#qmake ../dde-wm-chooser/
#make check
#qmake ../dmemory-warning-dialog/
#make check
#qmake ../dnetwork-secret-dialog/
#make check
#qmake ../dde-full-filesystem/
#make check
#qmake ../dde-bluetooth-dialog/
#make check
#qmake ../dde-notification-plugin/
#make check
#qmake ../dde-pixmix/
#make check
#qmake ../dde-touchscreen-dialog/
#make check

lcov -d ./ -c -o coverage_all.info
#lcov --extract coverage_all.info $EXTRACT_ARGS --output-file coverage.info
lcov --remove coverage_all.info "*/tests/*" "*/usr/include*" "*build/src*" "*persistence.cpp*" "*notifications_dbus_adaptor.cpp*" "*notifysettings.cpp*" "*dbuslogin1manager.cpp*" "*persistence.h*"  "*notifysettings.h*" "*dbuslogin1manager.h*" "*dbusdockinterface.h*" "*dbus_daemon_interface.h*" "*dbus_daemon_interface.cpp*" --output-file coverage.info
cd ..
genhtml -o $REPORT_DIR $BUILD_DIR/coverage.info
#rm -rf $BUILD_DIR
#rm -rf ../$BUILD_DIR
