QT       += core gui dbus x11extras svg sql dtkwidget testlib

TARGET = tst_dde-osd
TEMPLATE = app

include(../testcase.prf)

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
#    tst_actionbutton.cpp \
#    tst_appbody.cpp \
#    tst_appbodylabel.cpp \
#    tst_appicon.cpp \
#    tst_bubble.cpp \
#    tst_bubblemanager.cpp \
#    tst_bubbletool.cpp \
#    tst_button.cpp \
#    tst_iconbutton.cpp \
#    tst_notificationentity.cpp \
#    tst_notificationsetting.cpp \
#    tst_persistence.cpp \
#    tst_bubbleitem.cpp \
#    tst_bubbletitlewidget.cpp \
#    tst_notifylistview.cpp \
#    tst_notifywidget.cpp \
#    tst_overlapwidget.cpp \
#    tst_listview.cpp \
    ut_notifycenterwidget.cpp \
    ut_notifyListview.cpp \
    mockpersistence.cpp \
    ut_bubblemanager.cpp \
    mocknotifysetting.cpp \
    ut_manager.cpp \
    ut_dde-osd_main.cpp

INCLUDEPATH += ../../dde-osd/notification/ \
               ../../dde-osd/notification-center/ \
               ../../dde-osd/

RESOURCES +=

HEADERS += \
    mockpersistence.h \
    mocknotifysetting.h
