#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T17:37:11
#
#-------------------------------------------------
include(../common.pri)

QT       += core
QT       += gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-shutdown
TEMPLATE = app

PKGCONFIG += gsettings-qt
CONFIG += c++11 link_pkgconfig

include(./background/background.pri)

SOURCES += main.cpp\
    signalmanagement/signalmanager.cpp \
    dbus/dbussessionmanager.cpp \
    widgets/sessionbutton.cpp \
    view/contentwidget.cpp \
    view/mainframe.cpp \
    app/sessionmanagement.cpp \
    controller/dbusmanagement.cpp

HEADERS  += \
    signalmanagement/signalmanager.h \
    dbus/dbussessionmanager.h \
    widgets/sessionbutton.h \
    view/contentwidget.h \
    view/mainframe.h \
    app/sessionmanagement.h \
    controller/dbusmanagement.h

RESOURCES += \
    qss.qrc \
    image.qrc

target.path = $${PREFIX}/bin/
INSTALLS += target
