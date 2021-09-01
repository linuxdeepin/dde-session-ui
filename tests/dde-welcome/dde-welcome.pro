QT       += widgets core gui dbus gui-private testlib

TARGET = ut_dde-welcome
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
    widgets/ut_propertygroup.cpp \
    widgets/ut_fullscreenbackground.cpp \
    global_util/ut_publicfunc.cpp \
    global_util/ut_utilupdateui.cpp \
    global_util/ut_multiscreenmanager.cpp \
    ut_mainwidget.cpp \
    ut_updatecontent.cpp

INCLUDEPATH += ../../dde-welcome

RESOURCES +=
