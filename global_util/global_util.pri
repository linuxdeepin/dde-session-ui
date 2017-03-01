INCLUDEPATH += $$PWD

QT  += xml

HEADERS += \
    $$PWD/util_updateui.h \
    $$PWD/util_signalmanager.h \
    $$PWD/constants.h \
    $$PWD/util_settings.h \
    $$PWD/xkbparser.h \
    $$PWD/util_file.h \
    $$PWD/dbus/dbusdisplaymanager.h \
    $$PWD/dbus/dbuslogin1manager.h \
    $$PWD/dbus/dbusvariant.h \
    $$PWD/dbus/dbussessionmanager.h \
    $$PWD/dbus/displayinterface.h \
    $$PWD/dbus/dbushotzone.h \
    $$PWD/dbus/dbuslockservice.h \
    $$PWD/util_file.h \
    $$PWD/accountsutils.h \
    $$PWD/dbus/dbusaccounts.h \
    $$PWD/dbus/dbususer.h \
    $$PWD/dbus/dbusinputdevices.h \
    $$PWD/dbus/dbuscontrolcenter.h \
    $$PWD/dbus/dbuskeyboard.h \
    $$PWD/dbus/dbuslockfront.h \
    $$PWD/dbus/dbusmediaplayer2.h

SOURCES += \
    $$PWD/util_updateui.cpp \
    $$PWD/util_signalmanager.cpp \
    $$PWD/util_settings.cpp \
    $$PWD/xkbparser.cpp \
    $$PWD/util_file.cpp \
    $$PWD/dbus/dbusdisplaymanager.cpp \
    $$PWD/dbus/dbuslogin1manager.cpp \
    $$PWD/dbus/dbusvariant.cpp \
    $$PWD/dbus/dbussessionmanager.cpp \
    $$PWD/dbus/displayinterface.cpp \
    $$PWD/dbus/dbushotzone.cpp \
    $$PWD/dbus/dbuslockservice.cpp \
    $$PWD/accountsutils.cpp \
    $$PWD/dbus/dbusaccounts.cpp \
    $$PWD/dbus/dbususer.cpp \
    $$PWD/dbus/dbuskeyboard.cpp \
    $$PWD/dbus/dbusinputdevices.cpp \
    $$PWD/dbus/dbuscontrolcenter.cpp \
    $$PWD/dbus/dbuslockfront.cpp \
    $$PWD/dbus/dbusmediaplayer2.cpp

DISTFILES += \
    $$PWD/dbus/com.deepin.daemon.Zone.xml

