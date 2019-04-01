include(../common.pri)

QT       += core gui widgets dbus x11extras

TARGET = dde-full-filesystem
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11 link_pkgconfig dbus
PKGCONFIG += gsettings-qt xtst xext x11

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)

SOURCES += \
        main.cpp \
        fullfilesystem.cpp \
    fullfilesystemframe.cpp

HEADERS += \
        fullfilesystem.h \
    fullfilesystemframe.h

RESOURCES += \
    resources.qrc
