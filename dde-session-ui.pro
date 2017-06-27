TEMPLATE = subdirs
SUBDIRS += dde-shutdown \
           dde-lock \
           lightdm-deepin-greeter \
           dde-switchtogreeter \
           dde-lowpower \
           dde-osd \
#           dde-offline-upgrader \
           dde-suspend-dialog \
           dde-warning-dialog \
           dde-welcome \
           dde-wm-chooser

include(common.pri)
contains(DEFINES, ARCH_MIPSEL) {
    SUBDIRS += session-ui-guardien
}

!system($$PWD/translate_desktop.sh): error("Failed to generate translation")

# Automating generation .qm files from .ts files
!system($$PWD/translate_generation.sh): error("Failed to generate translation")

qm_files.path = /usr/share/dde-session-ui/translations/
qm_files.files = translations/*.qm

toggle_desktop.path = /usr/share/applications/
toggle_desktop.files = misc/applications/deepin-toggle-desktop.desktop

toogle_desktop_icon_48x48.path = /usr/share/icons/hicolor/48x48/apps/
toogle_desktop_icon_48x48.files = misc/icons/48x48/deepin-toggle-desktop.png
toogle_desktop_icon_64x64.path = /usr/share/icons/hicolor/64x64/apps/
toogle_desktop_icon_64x64.files = misc/icons/64x64/deepin-toggle-desktop.png
toogle_desktop_icon_96x96.path = /usr/share/icons/hicolor/96x96/apps/
toogle_desktop_icon_96x96.files = misc/icons/96x96/deepin-toggle-desktop.png
toogle_desktop_icon_scalable.path = /usr/share/icons/hicolor/scalable/apps/
toogle_desktop_icon_scalable.files = misc/icons/scalable/deepin-toggle-desktop.svg

INSTALLS += qm_files toggle_desktop toogle_desktop_icon_48x48 toogle_desktop_icon_64x64 toogle_desktop_icon_96x96 toogle_desktop_icon_scalable

