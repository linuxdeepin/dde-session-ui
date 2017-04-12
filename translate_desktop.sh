#!/bin/bash

DESKTOP_SOURCE_FILE=misc/applications/deepin-toggle-desktop.desktop.in
DESKTOP_DEST_FILE=misc/applications/deepin-toggle-desktop.desktop
DESKTOP_TS_DIR=translations/toggle-desktop/

/usr/bin/deepin-desktop-ts-convert ts2desktop $DESKTOP_SOURCE_FILE $DESKTOP_TS_DIR $DESKTOP_DEST_FILE
