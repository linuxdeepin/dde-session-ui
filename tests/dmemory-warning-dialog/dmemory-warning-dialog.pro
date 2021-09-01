QT       += widgets core gui dbus gui-private testlib

TARGET = ut_dmemory-warning-dialog
TEMPLATE = app

include(../testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../dmemory-warning-dialog/src/src.pri)

SOURCES += \
    main.cpp \
    ut_buttondelegate.cpp \
    ut_dmemorywarningdialogdaptor.cpp \
    ut_processinfotable.cpp \
    ut_dmemorywarningdialog.cpp \
    ut_processinfomanager.cpp

INCLUDEPATH += ../../dmemory-warning-dialog/src

RESOURCES +=
