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

SOURCES += main.cpp\
    dbus/dbussessionmanager.cpp \
    view/contentwidget.cpp \
    view/mainframe.cpp \
    controller/dbusmanagement.cpp \
    app/shutdownmanager.cpp

HEADERS  += \
    dbus/dbussessionmanager.h \
    view/contentwidget.h \
    view/mainframe.h \
    controller/dbusmanagement.h \
    app/shutdownmanager.h


RESOURCES += \
    qss.qrc \
    image.qrc

target.path = $${PREFIX}/bin/
INSTALLS += target
