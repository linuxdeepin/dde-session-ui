// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "utils.h"

const std::pair<QString, QString> GetSystemVersion()
{
    QSettings lsbSetting("/etc/deepin-version", QSettings::IniFormat);
    lsbSetting.setIniCodec("utf-8");
    lsbSetting.beginGroup("Release");
    QLocale locale;

    if (locale.language() == QLocale::Chinese)
        return std::pair<QString, QString>{ lsbSetting.value("Version").toString(),
                    lsbSetting.value(QString("Type[%1]").arg(locale.name()), "").toString() };

    return std::pair<QString, QString>{ lsbSetting.value("Version").toString(),
                lsbSetting.value(QString("Type"), "").toString() };
}
