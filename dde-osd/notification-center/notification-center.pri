isEmpty(PREFIX){
    PREFIX = /usr
}
QT += svg

HEADERS += \
    $$PWD/notifywidget.h \
    $$PWD/notifymodel.h \
    $$PWD/notifycenterwidget.h \
    $$PWD/bubbleitem.h \
    $$PWD/bubblegroup.h \
    $$PWD/appgroupmodel.h \
    $$PWD/appgroupdelegate.h \
    $$PWD/bubbledelegate.h \
    $$PWD/bubbleoverlapwidget.h \
    $$PWD/expandanimation.h \
    $$PWD/shortcutmanage.h \
    $$PWD/notifylistview.h

SOURCES += \
    $$PWD/notifywidget.cpp \
    $$PWD/notifymodel.cpp \
    $$PWD/notifycenterwidget.cpp \
    $$PWD/bubbleitem.cpp \
    $$PWD/bubblegroup.cpp \
    $$PWD/appgroupmodel.cpp \
    $$PWD/appgroupdelegate.cpp \
    $$PWD/bubbledelegate.cpp \
    $$PWD/bubbleoverlapwidget.cpp \
    $$PWD/expandanimation.cpp \
    $$PWD/shortcutmanage.cpp \
    $$PWD/notifylistview.cpp
