include(../common.pri)

load(dtk_qmake)

QT       += core gui widgets dbus x11extras

TARGET = dde-lock
TEMPLATE = app

CONFIG += c++11 link_pkgconfig dbus
PKGCONFIG += gsettings-qt xtst xext x11

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)

SOURCES += main.cpp \
    timewidget.cpp \
    lockframe.cpp \
    lockworker.cpp \
    dbus/dbuslockfrontservice.cpp \
    dbus/dbuslockagent.cpp

HEADERS  += \
    timewidget.h \
    lockframe.h \
    lockworker.h \
    dbus/dbuslockfrontservice.h \
    dbus/dbuslockagent.h

RESOURCES += \
    images.qrc \
    logintheme.qrc

DISTFILES += com.deepin.dde.lockFront.service

services.path = /usr/share/dbus-1/services
services.files = com.deepin.dde.lockFront.service

target.path = $${PREFIX}/bin/
INSTALLS += target services

host_mips64 {
    DEFINES += DISABLE_LOGIN_ANI
}

host_sw_64 {
    DEFINES += DISABLE_LOGIN_ANI
    DEFINES += SHENWEI_PLATFORM
}
