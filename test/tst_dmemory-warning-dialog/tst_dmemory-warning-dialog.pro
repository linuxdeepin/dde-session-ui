QT       += widgets core gui dbus gui-private

TARGET = tst_dmemmory-warning-dialog
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
    tst_dmemorywarningdialog.cpp \
    tst_processinfomanager.cpp \
    tst_processinfotable.cpp

INCLUDEPATH += ../../dmemory-warning-dialog/src

RESOURCES +=
