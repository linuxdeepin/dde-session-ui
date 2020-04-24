#This document shows you how to use the commands and parameters of dde-warning-dialog

command list:
    -t   show title             param: title name string
    -u   show english title     param: title name string
    -c   show content           param: the absolute path of document to show
    -e   show english content   param: the absolute path of document to show
    -a   show allow check       param: allow content
    -b   show english allow     param: allow content for english
    -d   hide bottom            param: if param is "yes", hide the bottom(include confirm button, cancel button and allow check) and show close buttom on  the top right corner of dialog, Otherwise, show bottom


for example:
1. -u  UOS隐私协议 -c /usr/share/deepin-deepinid-client/privacy/deepinid-CN-zh_CN.md -a 同意并加入用户体验计划 -d yes



