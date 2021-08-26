QT       += core gui dbus x11extras svg sql dtkwidget testlib

TARGET = tst_dde-osd
TEMPLATE = app

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt xcb-ewmh xrandr

CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest -lgmock -lX11 -lXext

DEFINES += QT_DEPRECATED_WARNINGS GTEST

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../dde-osd/notification/notification.pri)
include(../../dde-osd/notification-center/notification-center.pri)
include(../../dde-osd/src.pri)

SOURCES += \
    ut_notifycenterwidget.cpp \
    ut_notifyListview.cpp \
    mockpersistence.cpp \
    ut_bubblemanager.cpp \
    mocknotifysetting.cpp \
    ut_manager.cpp \
    ut_dde-osd_main.cpp \
    ut_actionbutton.cpp \
    ut_appbodylabel.cpp \
    ut_appbody.cpp \
    ut_appicon.cpp \
    ut_bubble.cpp \
    ut_bubbleitem.cpp \
    ut_bubbletitlewidget.cpp \
    ut_bubbletool.cpp \
    ut_button.cpp \
    ut_notificationentity.cpp \
    ut_overlapwidget.cpp \
    ut_listview.cpp \
    ut_notifywidget.cpp \
    ut_iconbutton.cpp \
    ut_icondata.cpp \
    ut_dockrect.cpp \
    ut_kblayoutindicator.cpp

INCLUDEPATH += ../../dde-osd/notification/ \
               ../../dde-osd/notification-center/ \
               ../../dde-osd/

RESOURCES +=

HEADERS += \
    mockpersistence.h \
    mocknotifysetting.h

# 执行 make check 会运行 tests
CONFIG += testcase no_testcase_installs
# 生成统计信息
QMAKE_CXXFLAGS += -g -Wall -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -Wall -fprofile-arcs -ftest-coverage  -O0
    

# 添加内存泄露检测
CONFIG(debug, debug|release) {
QMAKE_CXX += -g -fsanitize=address -O2
QMAKE_CXXFLAGS += -g -fsanitize=address -O2
QMAKE_LFLAGS += -g -fsanitize=address -O2
}