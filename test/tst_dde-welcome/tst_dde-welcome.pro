QT       += widgets core gui dbus gui-private

TARGET = tst_dde-welcome
TEMPLATE = app

include(../testcase.prf)

PKGCONFIG += dtkwidget dtkgui gio-qt dframeworkdbus gsettings-qt
CONFIG += c++11 link_pkgconfig
CONFIG -= app_bundle

LIBS += -lgtest

DEFINES += QT_DEPRECATED_WARNINGS

include(../../dde-welcome/src.pri)
include(../../widgets/widgets.pri)
include(../../global_util/global_util.pri)

SOURCES += \
    main.cpp \
    tst_mainwidget.cpp \
    tst_updatecontent.cpp

INCLUDEPATH += ../../dde-welcome

RESOURCES +=
