QT       += widgets core gui dbus gui-private concurrent

TARGET = ut_dde-license-dialog
TEMPLATE = app

include(../testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

include(../../dde-license-dialog/src.pri)

SOURCES += \
    main.cpp \
    ut_content.cpp \
    ut_mainwindow.cpp

INCLUDEPATH += ../../dde-license-dialog

RESOURCES +=
