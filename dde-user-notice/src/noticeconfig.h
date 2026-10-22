// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NOTICECONFIG_H
#define NOTICECONFIG_H

#include <DConfig>

#include <QString>
#include <QVariant>

// "用户须知"的配置项由 dde-session-ui 的 org.deepin.dde.session.ui schema 提供，
// 与 os-config 中的 override 使用同一套 key。
namespace UserNoticeConfig {

inline const QString AppId = QStringLiteral("org.deepin.dde.session.ui");
inline const QString KeyShowUserNotice = QStringLiteral("showUserNotice");
inline const QString KeyUserNoticePath = QStringLiteral("userNoticePath");

inline Dtk::Core::DConfig *config()
{
    static Dtk::Core::DConfig *cfg =
            Dtk::Core::DConfig::create(AppId, AppId, QString(), nullptr);
    return cfg;
}

inline QVariant value(const QString &key, const QVariant &fallback = QVariant())
{
    Dtk::Core::DConfig *cfg = config();
    return cfg ? cfg->value(key, fallback) : fallback;
}

inline QString noticePath()
{
    return value(KeyUserNoticePath, QString()).toString();
}

} // namespace UserNoticeConfig

#endif // NOTICECONFIG_H
