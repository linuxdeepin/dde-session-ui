isEmpty(PREFIX){
    PREFIX = /usr
}
QT += svg

HEADERS += \
    $$PWD/notifywidget.h \
    $$PWD/notifyview.h \
    $$PWD/notifycenterwidget.h \
    $$PWD/bubbleitem.h \
    $$PWD/bubblegroup.h

SOURCES += \
    $$PWD/notifywidget.cpp \
    $$PWD/notifyview.cpp \
    $$PWD/notifycenterwidget.cpp \
    $$PWD/bubbleitem.cpp \
    $$PWD/bubblegroup.cpp
