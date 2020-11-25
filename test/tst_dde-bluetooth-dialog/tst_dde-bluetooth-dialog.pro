QT       += widgets core gui dbus gui-private

TARGET = tst_dde-bluetooth-dialog
TEMPLATE = app

include(../dtk_testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

include(../../dde-bluetooth-dialog/src.pri)

SOURCES += \
    $$PWD/main.cpp \
    $$PWD/tst_pincodedialog.cpp

INCLUDEPATH += ../../dde-bluetooth-dialog

RESOURCES +=
