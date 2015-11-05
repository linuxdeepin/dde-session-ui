include(../common.pri)

QT       += core gui widgets dbus

TARGET = dde-lock
TEMPLATE = app

CONFIG += c++11 link_pkgconfig dbus
PKGCONFIG += gsettings-qt

include(../background/background.pri)
include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../cutelogger/cutelogger.pri)

SOURCES += main.cpp \
    lockframe.cpp \
    timewidget.cpp \
    controlwidget.cpp \
    dbus/dbuslockservice.cpp \
    dbus/dbuskeyboard.cpp \
    dbus/dbusinputdevices.cpp

HEADERS  += \
    lockframe.h \
    timewidget.h \
    controlwidget.h \
    dbus/dbuslockservice.h \
    dbus/dbusinputdevices.h \
    dbus/dbuskeyboard.h

target.path = $${PREFIX}/bin/
INSTALLS += target

RESOURCES += \
    qss.qrc \
    resources.qrc
