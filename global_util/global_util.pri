INCLUDEPATH += $$PWD

QT  += xml

HEADERS += \
    $$PWD/util_updateui.h \
    $$PWD/constants.h \
    $$PWD/xkbparser.h \
    $$PWD/dbus/dbusdisplaymanager.h \
    $$PWD/dbus/dbuslogin1manager.h \
    $$PWD/dbus/dbusvariant.h \
    $$PWD/dbus/dbussessionmanager.h \
    $$PWD/dbus/displayinterface.h \
    $$PWD/dbus/dbushotzone.h \
    $$PWD/dbus/dbuslockservice.h \
    $$PWD/dbus/dbusinputdevices.h \
    $$PWD/dbus/dbuscontrolcenter.h \
    $$PWD/dbus/dbuskeyboard.h \
    $$PWD/dbus/dbuslockfront.h \
    $$PWD/dbus/dbusmediaplayer2.h \
    $$PWD/public_func.h \
    $$PWD/multiscreenmanager.h

SOURCES += \
    $$PWD/util_updateui.cpp \
    $$PWD/xkbparser.cpp \
    $$PWD/dbus/dbusdisplaymanager.cpp \
    $$PWD/dbus/dbuslogin1manager.cpp \
    $$PWD/dbus/dbusvariant.cpp \
    $$PWD/dbus/dbussessionmanager.cpp \
    $$PWD/dbus/displayinterface.cpp \
    $$PWD/dbus/dbushotzone.cpp \
    $$PWD/dbus/dbuslockservice.cpp \
    $$PWD/dbus/dbuskeyboard.cpp \
    $$PWD/dbus/dbusinputdevices.cpp \
    $$PWD/dbus/dbuscontrolcenter.cpp \
    $$PWD/dbus/dbuslockfront.cpp \
    $$PWD/dbus/dbusmediaplayer2.cpp \
    $$PWD/public_func.cpp \
    $$PWD/multiscreenmanager.cpp

DISTFILES += \
    $$PWD/dbus/com.deepin.daemon.Zone.xml

