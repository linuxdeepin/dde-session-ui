TEMPLATE = subdirs
SUBDIRS += dde-shutdown \
           dde-lock \
           lightdm-deepin-greeter \
           dde-lockservice \
           dde-switchtogreeter \
           dde-lowpower \
           dde-osd \
           dde-zone \
           dde-offline-upgrader \
           dde-suspend-dialog \
           dde-warning-dialog

include(common.pri)
contains(DEFINES, ARCH_MIPSEL) {
    SUBDIRS += session-ui-guardien
}

system($$PWD/translate_desktop.sh)

# Automating generation .qm files from .ts files
system($$PWD/translate_generation.sh)

qm_files.path = /usr/share/dde-session-ui/translations/
qm_files.files = translations/*.qm

toggle_desktop.path = /usr/share/applications/
toggle_desktop.files = misc/applications/deepin-toggle-desktop.desktop

INSTALLS += qm_files toggle_desktop
