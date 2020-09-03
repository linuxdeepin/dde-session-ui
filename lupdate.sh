#!/bin/bash
rm ./translations/dde-session-ui.ts
lupdate ./ -ts -no-obsolete ./translations/dde-session-ui.ts
tx push -s -b klu
