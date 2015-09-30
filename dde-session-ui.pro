TEMPLATE = subdirs
SUBDIRS += dde-shutdown

TRANSLATIONS += translations/dde-session-ui.ts translations/dde-session-ui_zh_CN.ts

qm_files.files = translations/*.qm
qm_files.path = /usr/share/dde-session-ui/translations/

INSTALLS += qm_files
