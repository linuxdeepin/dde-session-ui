include(../common.pri)

QT       += core gui widgets dbus

TARGET = dde-lock
TEMPLATE = app

CONFIG += c++11 link_pkgconfig dbus
PKGCONFIG += gsettings-qt dtkbase dtkutil

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../boxframe/boxframe.pri)

SOURCES += main.cpp \
    timewidget.cpp \
    controlwidget.cpp \
    dbus/dbuskeyboard.cpp \
    dbus/dbusinputdevices.cpp \
    dbus/dbusmediaplayer2.cpp \
    ../widgets/dbus/dbuscontrolcenter.cpp \
    lockmanager.cpp \
    lockframe.cpp \
    dbus/dbuslockfrontservice.cpp \
    dbus/dbuslockfront.cpp

HEADERS  += \
    timewidget.h \
    controlwidget.h \
    dbus/dbusinputdevices.h \
    dbus/dbuskeyboard.h \
    dbus/dbusmediaplayer2.h \
    ../widgets/dbus/dbuscontrolcenter.h \
    lockmanager.h \
    lockframe.h \
    dbus/dbuslockfrontservice.h \
    dbus/dbuslockfront.h

RESOURCES += \
    images.qrc \
    logintheme.qrc

DISTFILES +=dde-lock.desktop \
    com.deepin.dde.lockFront.service

services.path = /usr/share/dbus-1/services
services.files = com.deepin.dde.lockFront.service

target.path = $${PREFIX}/bin/
INSTALLS += target services

isEqual(LOCK_NO_QUIT, YES) {
    DEFINES +=LOCK_NO_QUIT

    #let dde-lock to be auto-start
    desktop_file.files = dde-lock.desktop
    desktop_file.path = /etc/xdg/autostart/
    INSTALLS += desktop_file
}





