#!/bin/bash
lupdate ./ -ts ./translations/dde-session-ui.ts
tx push -s -b master
