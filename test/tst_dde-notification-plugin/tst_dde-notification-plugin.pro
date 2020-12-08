QT       += widgets core gui dbus gui-private svg

TARGET = tst_dde-notification-plugin
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
    tst_notificationswidget.cpp \
    tst_notificationsplugin.cpp

INCLUDEPATH += ../../dde-notification-plugin \
               /usr/include/dde-dock

RESOURCES +=
