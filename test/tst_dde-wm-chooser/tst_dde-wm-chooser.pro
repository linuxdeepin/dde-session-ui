QT       += widgets core gui dbus gui-private

TARGET = tst_dde-wm-chooser
TEMPLATE = app

include(../dtk_testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../../dde-wm-chooser/src.pri)
include(../../widgets/widgets.pri)
include(../../global_util/global_util.pri)

SOURCES += \
    main.cpp \
    tst_wmchooser.cpp \
    tst_wmframe.cpp

INCLUDEPATH += ../../dde-wm-chooser

RESOURCES +=
