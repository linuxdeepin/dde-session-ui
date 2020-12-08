QT       += widgets core gui dbus gui-private

TARGET = tst_dde-suspend-dialog
TEMPLATE = app

include(../testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../dde-suspend-dialog/src.pri)

SOURCES += \
    main.cpp \
    tst_suspenddialog.cpp

INCLUDEPATH += ../../dde-suspend-dialog

RESOURCES +=
