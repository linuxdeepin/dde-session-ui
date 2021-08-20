QT       += core gui widgets dbus dtkwidget

TARGET = dmemory-warning-dialog
TEMPLATE = app

CONFIG += c++14 link_pkgconfig
PKGCONFIG += dframeworkdbus

HEADERS += \
    src/buttondelegate.h \
    src/dmemorywarningdialog.h \
    src/processinfodelegate.h \
    src/processinfomanager.h \
    src/processinfomodel.h \
    src/processinfotable.h \
    src/dmemorywarningdialogadaptor.h \
    ../common/accessibilitycheckerex.h

SOURCES += \
    src/buttondelegate.cpp \
    src/dmemorywarningdialog.cpp \
    src/main.cpp \
    src/processinfodelegate.cpp \
    src/processinfomanager.cpp \
    src/processinfomodel.cpp \
    src/processinfotable.cpp \
    src/dmemorywarningdialogadaptor.cpp \
    ../common/accessibilitycheckerex.cpp

service.path = /usr/share/dbus-1/services
service.files = com.deepin.dde.MemoryWarningDialog.service

target.path = /usr/bin
INSTALLS   += target service
