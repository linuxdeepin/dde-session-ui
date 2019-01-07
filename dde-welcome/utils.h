#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <QString>
#include <QSettings>
#include <QLocale>

static const std::pair<QString, QString> GetSystemVersion()
{
    QSettings lsbSetting("/etc/deepin-version", QSettings::IniFormat);
    lsbSetting.setIniCodec("utf-8");
    lsbSetting.beginGroup("Release");
    QLocale locale;

    if (locale.language() == QLocale::Chinese)
        return std::pair<QString, QString> {
            lsbSetting.value("Version").toString(),
                    lsbSetting.value(QString("Type[%1]").arg(locale.name()), "").toString()
        };

    return std::pair<QString, QString> {
        lsbSetting.value("Version").toString(),
                lsbSetting.value(QString("Type"), "").toString()
    };
}

static const QString GetLocalVersion() {
    QSettings welcomeSetting("deepin", "dde-welcome");
    return welcomeSetting.value("Version").toString();
}

static bool CheckVersionChanged()
{
    const QString currentVersion = GetSystemVersion().first;

    if (currentVersion.isEmpty())
        return false;

    // check file exist
    QSettings welcomeSetting("deepin", "dde-welcome");
    QString version = welcomeSetting.value("Version").toString();
    if (version.isEmpty()) {
        // If the local file does not exist, do not prompt.
        // In some distributions, the first use prompts an upgrade.
        welcomeSetting.setValue("Version", currentVersion);
        return false;
    }

    welcomeSetting.setValue("Version", currentVersion);

    return version != currentVersion;
}

#endif // UTILS_H
