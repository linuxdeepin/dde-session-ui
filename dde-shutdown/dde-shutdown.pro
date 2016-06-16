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

PKGCONFIG += gsettings-qt liblightdm-qt5-3 dtkbase dtkutil
CONFIG += c++11 link_pkgconfig

include(../widgets/widgets.pri)
include(../global_util/global_util.pri)
include(../boxframe/boxframe.pri)

SOURCES += main.cpp \
    view/contentwidget.cpp \
    view/mainframe.cpp \
    app/shutdownmanager.cpp \
    ../widgets/dbus/dbuscontrolcenter.cpp \
    app/shutdownframe.cpp \
    view/multiuserswarningview.cpp 

HEADERS  += \
    view/contentwidget.h \
    view/mainframe.h \
    app/shutdownmanager.h \
    ../widgets/dbus/dbuscontrolcenter.h \
    app/shutdownframe.h \
    view/multiuserswarningview.h \
    common.h


RESOURCES += \
    qss.qrc \
    image.qrc

DISTFILES += com.deepin.dde.shutdownFront.service

target.path = $${PREFIX}/bin/
INSTALLS += target
service_file.path = /usr/share/dbus-1/services/
service_file.files = com.deepin.dde.shutdownFront.service
INSTALLS += service_file

contains(DEFINES, ARCH_MIPSEL) {
    DEFINES += SHUTDOWN_NO_QUIT
}
