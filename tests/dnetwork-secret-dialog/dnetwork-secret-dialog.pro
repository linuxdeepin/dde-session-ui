QT       += widgets core gui dbus gui-private

TARGET = ut_dnetwork-secret-dialog
TEMPLATE = app

include(../testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../dnetwork-secret-dialog/src.pri)

SOURCES += \
    main.cpp \
    ut_networksecretdialog.cpp

INCLUDEPATH += ../../dnetwork-secret-dialog

RESOURCES +=
