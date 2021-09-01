QT       += widgets core gui dbus gui-private svg testlib

TARGET = ut_dde-notification-plugin
TEMPLATE = app

include(../testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../dde-notification-plugin/src.pri)

SOURCES += main.cpp \
    ut_notificationsplugin.cpp \
    ut_notificationswidget.cpp

INCLUDEPATH += ../../dde-notification-plugin \
               /usr/include/dde-dock

RESOURCES +=
