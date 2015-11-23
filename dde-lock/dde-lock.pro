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
include(../boxframe/boxframe.pri)

SOURCES += main.cpp \
    timewidget.cpp \
    controlwidget.cpp \
    dbus/dbuskeyboard.cpp \
    dbus/dbusinputdevices.cpp \
    dbus/dbusmediaplayer2.cpp \
    ../widgets/dbus/dbuscontrolcenter.cpp \
    lockmanager.cpp \
    lockframe.cpp

HEADERS  += \
    timewidget.h \
    controlwidget.h \
    dbus/dbusinputdevices.h \
    dbus/dbuskeyboard.h \
    dbus/dbusmediaplayer2.h \
    ../widgets/dbus/dbuscontrolcenter.h \
    lockmanager.h \
    lockframe.h

target.path = $${PREFIX}/bin/
INSTALLS += target

RESOURCES += \
    images.qrc \
    logintheme.qrc
