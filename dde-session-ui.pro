TEMPLATE = subdirs
SUBDIRS += lightdm-deepin-greeter \
           dde-shutdown \
           dde-lock

TRANSLATIONS += translations/dde-session-ui.ts translations/dde-session-ui_zh_CN.ts

qm_files.path = /usr/share/dde-session-ui/translations/
qm_files.files = translations/*.qm

INSTALLS += qm_files
