QT       += widgets core gui dbus gui-private

TARGET = tst_dde-license-dialog
TEMPLATE = app

include(../dtk_testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

include(../../dde-license-dialog/src.pri)

SOURCES += \
    main.cpp \
    tst_content.cpp \
    tst_mainwindow.cpp

INCLUDEPATH += ../../dde-license-dialog

RESOURCES +=
