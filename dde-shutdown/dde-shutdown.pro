#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T17:37:11
#
#-------------------------------------------------
include(../common.pri)

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-shutdown
TEMPLATE = app

PKGCONFIG += gsettings-qt
CONFIG += c++11 link_pkgconfig

include(../background/background.pri)
include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../boxframe/boxframe.pri)
include(../cutelogger/cutelogger.pri)

SOURCES += main.cpp \
    view/contentwidget.cpp \
    view/mainframe.cpp \
    app/shutdownmanager.cpp \
    ../widgets/dbus/dbuscontrolcenter.cpp \
    app/shutdownframe.cpp

HEADERS  += \
    view/contentwidget.h \
    view/mainframe.h \
    app/shutdownmanager.h \
    ../widgets/dbus/dbuscontrolcenter.h \
    app/shutdownframe.h


RESOURCES += \
    qss.qrc \
    image.qrc

target.path = $${PREFIX}/bin/
INSTALLS += target

isEqual(DISABLE_SUSPEND, YES) {
    DEFINES += DISABLE_SUSPEND
}
