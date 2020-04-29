isEmpty(PREFIX){
    PREFIX = /usr
}
QT += svg

HEADERS += \
    $$PWD/notifywidget.h \
    $$PWD/notifycenterwidget.h \
    $$PWD/bubbleitem.h \
    $$PWD/notifylistview.h \
    $$PWD/itemdelegate.h \
    $$PWD/notifymodel.h \
    $$PWD/bubbletitlewidget.h \
    $$PWD/overlapwidet.h

SOURCES += \
    $$PWD/notifywidget.cpp \
    $$PWD/notifycenterwidget.cpp \
    $$PWD/bubbleitem.cpp \
    $$PWD/notifylistview.cpp \
    $$PWD/itemdelegate.cpp \
    $$PWD/notifymodel.cpp \
    $$PWD/bubbletitlewidget.cpp \
    $$PWD/overlapwidet.cpp
