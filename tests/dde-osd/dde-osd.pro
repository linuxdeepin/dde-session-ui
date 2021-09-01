QT       += core gui dbus x11extras svg sql dtkwidget testlib

TARGET = ut_dde-osd
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
include(../testcase.prf)

SOURCES += \
    mockpersistence.cpp \
    mocknotifysetting.cpp \
    notification/ut_actionbutton.cpp \
    notification/ut_appbodylabel.cpp \
    notification/ut_appbody.cpp \
    notification/ut_appicon.cpp \
    notification/ut_bubble.cpp \
    notification/ut_bubblemanager.cpp \
    notification/ut_bubbletool.cpp \
    notification/ut_button.cpp \
    notification/ut_notificationentity.cpp \
    notification/ut_iconbutton.cpp \
    notification/ut_dockrect.cpp \
    notification-center/ut_bubbleitem.cpp \
    notification-center/ut_bubbletitlewidget.cpp \
    notification-center/ut_overlapwidget.cpp \
    notification-center/ut_notifywidget.cpp \
    notification-center/ut_notifycenterwidget.cpp \
    notification-center/ut_notifyListview.cpp \
    ut_manager.cpp \
    ut_dde-osd_main.cpp \
    ut_listview.cpp \
    ut_audioprovider.cpp \
    ut_brightnessprovider.cpp \
    ut_displaymodeprovider.cpp \
    ut_indicatorprovider.cpp \
    ut_kblayoutprovider.cpp \
    ut_common.cpp \
    ut_delegate.cpp \
    ut_container.cpp \
    ut_osdprovider.cpp

INCLUDEPATH += ../../dde-osd/notification/ \
               ../../dde-osd/notification-center/ \
               ../../dde-osd/

RESOURCES +=

HEADERS += \
    mockpersistence.h \
    mocknotifysetting.h
