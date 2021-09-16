QT += core gui widgets dbus x11extras testlib

TARGET = ut_dde-full-filesystem
TEMPLATE = app

PKGCONFIG += gsettings-qt xtst xext x11

CONFIG += c++11 link_pkgconfig dbus
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lgtest

include(../../dde-full-filesystem/src.pri)
include(../../widgets/widgets.pri)
include(../../global_util/global_util.pri)
include(../testcase.prf)

SOURCES += \
        main.cpp \
    ut_fullfilesystem.cpp \
    ut_fullfilesystemframe.cpp

INCLUDEPATH += ../../dde-full-filesystem

RESOURCES +=
