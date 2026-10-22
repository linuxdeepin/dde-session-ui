#!/bin/bash
# SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
# SPDX-License-Identifier: GPL-3.0-or-later
#
# 供 greeter 的 pre-greeter 调用：返回 0 表示需要显示"用户须知"

if ! userNoticePath=$(dde-dconfig --get -a org.deepin.dde.session.ui -r org.deepin.dde.session.ui -k userNoticePath); then
    echo "Failed to read userNoticePath from dde-dconfig; skipping user notice" >&2
    exit 2
fi

if ! showUserNotice=$(dde-dconfig --get -a org.deepin.dde.session.ui -r org.deepin.dde.session.ui -k showUserNotice); then
    echo "Failed to read showUserNotice from dde-dconfig; skipping user notice" >&2
    exit 2
fi

userNoticePath=${userNoticePath//\"/}

if [ "${showUserNotice}" = "false" ]; then
    echo "can not showUserNotice"
    exit 1
fi

if [ ! -s "${userNoticePath}" ] || [ ! -r "${userNoticePath}" ]; then
    echo "Configured user notice '${userNoticePath}' is empty, missing, or unreadable; skipping user notice" >&2
    exit 2
fi

echo "${userNoticePath} exist, system update can proceed"
exit 0
