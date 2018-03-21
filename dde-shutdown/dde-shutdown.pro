#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T17:37:11
#
#-------------------------------------------------
include(../common.pri)

QT       += core gui dbus svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-shutdown
TEMPLATE = app

PKGCONFIG += dframeworkdbus dtkcore
CONFIG += c++11 link_pkgconfig

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../boxframe/boxframe.pri)

SOURCES += main.cpp \
    view/contentwidget.cpp \
    view/multiuserswarningview.cpp \
    app/shutdownframe.cpp \
    view/inhibitwarnview.cpp \
    view/systemmonitor.cpp

HEADERS  += \
    common.h \
    view/contentwidget.h \
    view/multiuserswarningview.h \
    app/shutdownframe.h \
    view/inhibitwarnview.h \
    view/systemmonitor.h \
    view/warningview.h

RESOURCES += \
    qss.qrc \
    image.qrc

DISTFILES += com.deepin.dde.shutdownFront.service

target.path = $${PREFIX}/bin/
INSTALLS += target
service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.shutdownFront.service
INSTALLS += service_file

isEqual(SHUTDOWN_NO_QUIT, YES){
    DEFINES += SHUTDOWN_NO_QUIT
}
