QT       += core gui widgets dbus

TARGET = dmemory-warning-dialog
TEMPLATE = app

CONFIG += c++14 link_pkgconfig
PKGCONFIG += dtkwidget 

HEADERS += \
    src/buttondelegate.h \
    src/dmemorywarningdialog.h \
    src/processinfodelegate.h \
    src/processinfomanager.h \
    src/processinfomodel.h \
    src/processinfotable.h \
    src/dmemorywarningdialogadaptor.h

SOURCES += \
    src/buttondelegate.cpp \
    src/dmemorywarningdialog.cpp \
    src/main.cpp \
    src/processinfodelegate.cpp \
    src/processinfomanager.cpp \
    src/processinfomodel.cpp \
    src/processinfotable.cpp \
    src/dmemorywarningdialogadaptor.cpp

service.path = /usr/share/dbus-1/services
service.files = com.deepin.dde.MemoryWarningDialog.service

target.path = /usr/bin
INSTALLS   += target service
