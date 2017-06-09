include(../common.pri)

QT       += core gui widgets dbus x11extras

TARGET = dde-lock
TEMPLATE = app

CONFIG += c++11 link_pkgconfig dbus
PKGCONFIG += gsettings-qt dtkbase dtkutil xtst xext x11

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../boxframe/boxframe.pri)

SOURCES += main.cpp \
    timewidget.cpp \
    controlwidget.cpp \
    lockmanager.cpp \
    lockframe.cpp \
    dbus/dbuslockfrontservice.cpp

HEADERS  += \
    timewidget.h \
    controlwidget.h \
    lockmanager.h \
    lockframe.h \
    dbus/dbuslockfrontservice.h

RESOURCES += \
    images.qrc \
    logintheme.qrc

DISTFILES += com.deepin.dde.lockFront.service

services.path = /usr/share/dbus-1/services
services.files = com.deepin.dde.lockFront.service

target.path = $${PREFIX}/bin/
INSTALLS += target services

isEqual(LOCK_NO_QUIT, YES) {
    DEFINES +=LOCK_NO_QUIT
}

