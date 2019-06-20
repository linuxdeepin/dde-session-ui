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

SOURCES += main.cpp \
    shutdownworker.cpp \
    view/contentwidget.cpp \
    view/multiuserswarningview.cpp \
    app/shutdownframe.cpp \
    view/inhibitwarnview.cpp \
    view/systemmonitor.cpp \
    dbusshutdownagent.cpp

HEADERS  += \
    common.h \
    shutdownworker.h \
    view/contentwidget.h \
    view/multiuserswarningview.h \
    app/shutdownframe.h \
    view/inhibitwarnview.h \
    view/systemmonitor.h \
    view/warningview.h \
    dbusshutdownagent.h

RESOURCES += \
    qss.qrc \
    image.qrc

DISTFILES += com.deepin.dde.shutdownFront.service

ENABLE_SYSTEM_MONITOR=true

config_file_tmp.input = $$PWD/../files/dde-shutdown.conf.in
config_file_tmp.output = $$OUT_PWD/../files/dde-shutdown.conf

QMAKE_SUBSTITUTES += config_file_tmp
QMAKE_CLEAN       += $${config_file_tmp.output}

config_file.path = $$PREFIX/share/dde-session-ui/
config_file.files += $$OUT_PWD/../files/dde-shutdown.conf

target.path = $${PREFIX}/bin/
INSTALLS += target
service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.shutdownFront.service
INSTALLS += service_file config_file
