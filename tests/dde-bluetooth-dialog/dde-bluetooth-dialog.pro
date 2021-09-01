QT       += widgets core gui dbus gui-private

TARGET = ut_dde-bluetooth-dialog
TEMPLATE = app

include(../testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

include(../../dde-bluetooth-dialog/src.pri)

SOURCES += \
    ut_pincodedialog.cpp \
    main.cpp

INCLUDEPATH += ../../dde-bluetooth-dialog

RESOURCES +=
