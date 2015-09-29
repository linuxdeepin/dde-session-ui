#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T17:37:11
#
#-------------------------------------------------

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
    user/imagebutton.cpp \
    user/userframe.cpp \
    user/userwidget.cpp \
    widgets/sessionbutton.cpp \
    widgets/passwdedit.cpp \
    view/contentwidget.cpp \
    view/leftframe.cpp \
    view/leftcornerwidget.cpp \
    view/powermenuframe.cpp \
    view/mainframe.cpp \
    app/sessionmanagement.cpp \
    controller/dbusmanagement.cpp

HEADERS  += \
    signalmanagement/signalmanager.h \
    dbus/dbussessionmanager.h \
    user/imagebutton.h \
    user/userframe.h \
    user/userwidget.h \
    widgets/sessionbutton.h \
    widgets/passwdedit.h \
    view/contentwidget.h \
    view/leftcornerwidget.h \
    view/leftframe.h \
    view/mainframe.h \
    view/powermenuframe.h \
    app/sessionmanagement.h \
    controller/dbusmanagement.h

RESOURCES += \
    image.qrc \
    qss.qrc
