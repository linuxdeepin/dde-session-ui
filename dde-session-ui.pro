#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T17:37:11
#
#-------------------------------------------------

QT       += core
QT       += gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-session-ui
TEMPLATE = app

PKGCONFIG += gsettings-qt
CONFIG += c++11 link_pkgconfig


include(./background/background.pri)

SOURCES += main.cpp\
    mainframe.cpp \
    powermenuframe.cpp \
    dbusmanagement.cpp \
    signalmanagement/signalmanager.cpp \
    sessionmanagement.cpp \
    dbus/dbussessionmanager.cpp \
    user/imagebutton.cpp \
    user/userframe.cpp \
    user/userwidget.cpp \
    widgets/sessionbutton.cpp \
    widgets/passwdedit.cpp \
    view/contentwidget.cpp \
    view/leftframe.cpp \
    view/leftcornerwidget.cpp

HEADERS  += \
    powermenuframe.h \
    mainframe.h \
    dbusmanagement.h \
    signalmanagement/signalmanager.h \
    sessionmanagement.h \
    dbus/dbussessionmanager.h \
    user/imagebutton.h \
    user/userframe.h \
    user/userwidget.h \
    widgets/sessionbutton.h \
    widgets/passwdedit.h \
    view/contentwidget.h \
    view/leftcornerwidget.h \
    view/leftframe.h

DISTFILES +=

RESOURCES += \
    image.qrc \
    qss.qrc
