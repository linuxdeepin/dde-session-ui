#-------------------------------------------------
#
# Project created by QtCreator 2015-09-06T17:37:11
#
#-------------------------------------------------

QT       += core
QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SessionManager
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        sessionmanager.cpp \
    sessionbutton.cpp \
    passwdedit.cpp \
    leftframe.cpp \
    leftcornerwidget.cpp \
    contentwidget.cpp \
    mainframe.cpp \
    powermenuframe.cpp

HEADERS  += sessionmanager.h \
    sessionbutton.h \
    passwdedit.h \
    leftframe.h \
    leftcornerwidget.h \
    contentwidget.h \
    mainframe.h \
    powermenuframe.h

DISTFILES +=

RESOURCES += \
    image.qrc \
    qss.qrc
