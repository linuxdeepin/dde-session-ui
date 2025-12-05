// SPDX-FileCopyrightText: 2021 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QCoreApplication>
#include <QDateTime>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QDBusServiceWatcher>
#include <QNetworkInterface>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <DConfig>

#include <unistd.h>

#include "loginreminderinfo.h"

DCORE_USE_NAMESPACE

// 前置声明
int checkPasswordExpired();

int start()
{
    qDebug() << "login reminder init";
    
    // 读取 DConfig 配置，检查 LoginReminder 是否开启
    DConfig *config = DConfig::create("org.deepin.login-reminder", "org.deepin.login-reminder");
    bool loginReminderEnabled = false; // 默认关闭
    if (config && config->isValid()) {
        loginReminderEnabled = config->value("LoginReminder", false).toBool();
        qDebug() << "LoginReminder enabled:" << loginReminderEnabled;
    }
    
    // 如果 login-reminder 没有开启，仅检查密码过期情况
    if (!loginReminderEnabled) {
        qDebug() << "LoginReminder is disabled, only checking password expiration";
        return checkPasswordExpired();
    }

    QDBusInterface userInter("org.deepin.dde.Accounts1", QString("/org/deepin/dde/Accounts1/User%1").arg(getuid()), "org.deepin.dde.Accounts1.User", QDBusConnection::systemBus());
    QDBusPendingReply<LoginReminderInfo> reply = userInter.call("GetReminderInfo");
    if (reply.isError()) {
        qWarning() << "failed to retrieve login reminder info, error: " << reply.error().message();
        return -1;
    }

    // 获取当前设备的ip
    auto getFirstIpAddress = [] {
        QList<QHostAddress> addressesList = QNetworkInterface::allAddresses();

        // 优先查找ipv4的地址
        auto ipV4Iter = std::find_if(addressesList.begin(), addressesList.end(), [ = ] (QHostAddress address) {
            return (address.protocol() == QAbstractSocket::IPv4Protocol)
                    && (address != QHostAddress::LocalHost)
                    && (address != QHostAddress::Broadcast)
                    && (address != QHostAddress::AnyIPv4)
                    && (address != QHostAddress::Any);
        });

        if (ipV4Iter != addressesList.end()) {
            return ipV4Iter->toString();
        }

        // 其次,寻找可用的ipv6地址
        auto ipV6Iter = std::find_if(addressesList.begin(), addressesList.end(), [ = ] (QHostAddress address) {
            return (address.protocol() == QAbstractSocket::IPv6Protocol)
                    && (address != QHostAddress::LocalHostIPv6)
                    && (address != QHostAddress::AnyIPv6)
                    && (address != QHostAddress::Any);
        });

        if (ipV6Iter != addressesList.end()) {
            return ipV6Iter->toString();
        }

        return QString();
    };

#define SECONDS_PER_DAY (60 * 60 * 24)
    const LoginReminderInfo &info = reply.value();

    const QString &title = QObject::tr("Login Reminder");
    const QString &currentLoginTime = info.CurrentLogin.Time.left(QString("yyyy-MM-dd hh:mm:ss").length());
    const QString &address = (info.CurrentLogin.Address == "0.0.0.0") ? getFirstIpAddress() : info.CurrentLogin.Address;
    QString body = QString("%1 %2 %3").arg(info.Username).arg(address).arg(currentLoginTime);
    int daysLeft = info.spent.LastChange + info.spent.Max - int(QDateTime::currentSecsSinceEpoch() / SECONDS_PER_DAY);
    bool needNotify = false;
    if ((info.spent.Max != -1) && (info.spent.Warn != -1) && (info.spent.Warn > daysLeft)) {
        needNotify = true;
        body += " " + QString(QObject::tr("Your password will expire in %1 days")).arg(daysLeft);
    }
    if (info.FailCountSinceLastLogin > 0) {
        needNotify = true;
        body += "\n" + QString(QObject::tr("%1 login failures since the last successful login")).arg(info.FailCountSinceLastLogin);
    }
    if (!needNotify) {
        return 0;
    }
    const QString &lastLoginTime = info.LastLogin.Time.left(QString("yyyy-MM-dd hh:mm:ss").length());
    QString content;
    content += QString("<p>%1</p>").arg(info.Username);
    content += QString("<p>%1</p>").arg(address);
    content += "<p>" + QString(QObject::tr("Login time: %1")).arg(currentLoginTime) + "</p>";
    content += "<p>" + QString(QObject::tr("Last login: %1")).arg(lastLoginTime) + "</p>";
    content += "<p><b>" + QString("Your password will expire in %1 days").arg(daysLeft) + "</b></p>";
    content += "<br>";
    content += "<p>" + QString("%1 login failures since the last successful login").arg(info.FailCountSinceLastLogin) + "</p>";

    QDBusInterface *notifyInter = new QDBusInterface("org.freedesktop.Notifications", "/org/freedesktop/Notifications", "org.freedesktop.Notifications");
    QVariantMap hints;
    hints.insert(QString("x-deepin-action-details"),QVariant(QString("/usr/bin/dde-hints-dialog, %1, %2").arg(title).arg(content)));
    QDBusPendingReply<uint> notifyIdReply = notifyInter->call("Notify", "dde-control-center", uint(0), "preferences-system", title, body, QStringList() << "details" << QObject::tr("Details"), hints, int(0));
    if (notifyIdReply.isError()) {
        qWarning() << "failed to call notification, error: " << notifyIdReply.error().message();
        return -1;
    }
    uint notifyId = notifyIdReply.value();

    // 5秒后自动关闭通知，避免在通知中心中显示
    sleep(10);

    QDBusPendingReply<> closeReply = notifyInter->call("CloseNotification", notifyId);
    if (closeReply.isError()) {
        qWarning() << "failed to close notification, error: " << closeReply.error().message();
        return -1;
    }

    qDebug() << "login reminder init finished";
    return 0;
}

int checkPasswordExpired() {
    qDebug() << "check password expired";
    
    // 调用 PasswordExpiredInfo 获取密码过期信息
    QDBusInterface userInter("org.deepin.dde.Accounts1", 
                             QString("/org/deepin/dde/Accounts1/User%1").arg(getuid()), 
                             "org.deepin.dde.Accounts1.User", 
                             QDBusConnection::systemBus());
    
    QDBusMessage reply = userInter.call("PasswordExpiredInfo");
    if (reply.type() == QDBusMessage::ErrorMessage) {
        qWarning() << "failed to retrieve password expired info, error: " << reply.errorMessage();
        return -1;
    }

    // 解析返回值: (Int32 expiredStatus, Int64 dayLeft)
    QList<QVariant> args = reply.arguments();
    if (args.size() < 2) {
        qWarning() << "invalid reply from PasswordExpiredInfo";
        return -1;
    }

    int expiredStatus = args[0].toInt();
    qint64 dayLeft = args[1].toLongLong();

    qDebug() << "Password expired status:" << expiredStatus << ", days left:" << dayLeft;

    // expiredStatus: 0=正常, 1=即将过期, 2=已过期
    if (expiredStatus == 0) {
        qDebug() << "Password is valid, no notification needed";
        return 0;
    }

    // 构建通知内容
    QString title = QObject::tr("Password Expiration Warning");
    QString body;
    QString content;

    if (expiredStatus == 2) {
        // 密码已过期
        body = QObject::tr("Your password has expired. Please change it immediately.");
        content = QString("<p><b>%1</b></p>").arg(QObject::tr("Your password has expired!"));
        content += QString("<p>%1</p>").arg(QObject::tr("For security reasons, please change your password immediately."));
    } else if (expiredStatus == 1 && dayLeft > 0) {
        // 密码即将过期
        body = QObject::tr("Your password will expire in %1 days. Please change it soon.").arg(dayLeft);
        content = QString("<p><b>%1</b></p>").arg(QObject::tr("Password Expiration Warning"));
        content += QString("<p>%1</p>").arg(QObject::tr("Your password will expire in %1 days.").arg(dayLeft));
        content += QString("<p>%1</p>").arg(QObject::tr("Please change your password as soon as possible."));
    } else {
        // 其他情况
        qDebug() << "Unknown password status, no notification";
        return 0;
    }

    // 发送通知
    QDBusInterface *notifyInter = new QDBusInterface("org.freedesktop.Notifications", 
                                                      "/org/freedesktop/Notifications", 
                                                      "org.freedesktop.Notifications");
    QVariantMap hints;
    // 点击通知后打开控制中心的账户密码修改页面
    hints.insert(QString("x-deepin-action-change_password"), 
                 QVariant(QString("dbus-send,--print-reply,--dest=org.deepin.dde.ControlCenter1,/org/deepin/dde/ControlCenter1,org.deepin.dde.ControlCenter1.ShowPage,string:accountsloginMethodItempassword")));
    
    QDBusPendingReply<uint> notifyIdReply = notifyInter->call("Notify", 
                                                                "dde-control-center", 
                                                                uint(0), 
                                                                "preferences-system", 
                                                                title, 
                                                                body, 
                                                                QStringList() << "change_password" << QObject::tr("Change Password"), 
                                                                hints, 
                                                                int(0));
    if (notifyIdReply.isError()) {
        qWarning() << "failed to call notification, error: " << notifyIdReply.error().message();
        return -1;
    }
    uint notifyId = notifyIdReply.value();

    // 10秒后自动关闭通知
    sleep(5);

    QDBusPendingReply<> closeReply = notifyInter->call("CloseNotification", notifyId);
    if (closeReply.isError()) {
        qWarning() << "failed to close notification, error: " << closeReply.error().message();
        return -1;
    }

    qDebug() << "password expired check finished";
    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("dde-login-reminder");

    // 加载翻译文件 - 使用 dde-session-ui 的主翻译文件
    QTranslator *translator = new QTranslator(&app);
    QString locale = QLocale::system().name();
    
    // 尝试多个可能的翻译路径
    QStringList translationPaths;
    translationPaths << QString(CMAKE_INSTALL_FULL_DATADIR) + "/dde-session-ui/translations"
                     << "/usr/share/dde-session-ui/translations"
                     << "/usr/local/share/dde-session-ui/translations";
    
    bool loaded = false;
    for (const QString &path : translationPaths) {
        if (translator->load(QString("dde-session-ui_%1").arg(locale), path)) {
            app.installTranslator(translator);
            qDebug() << "Loaded translation for locale:" << locale << "from" << path;
            loaded = true;
            break;
        }
    }
    
    if (!loaded) {
        qDebug() << "Failed to load translation for locale:" << locale;
        qDebug() << "Tried paths:" << translationPaths;
    }

    registerLoginReminderInfoMetaType();

    // 检查 org.deepin.dde.Notification1 服务是否已启动
    QDBusConnectionInterface *interface = QDBusConnection::sessionBus().interface();
    if (interface->isServiceRegistered("org.deepin.dde.Notification1")) {
        // 服务已启动，直接执行
        qDebug() << "Notification service already available, starting immediately";
        int result = start();
        return result;
    }

    // 服务未启动，动态监听服务注册
    qDebug() << "Notification service not available, waiting for it to start...";
    QDBusServiceWatcher *watcher = new QDBusServiceWatcher(
        "org.deepin.dde.Notification1",
        QDBusConnection::sessionBus(),
        QDBusServiceWatcher::WatchForRegistration,
        &app
    );

    QObject::connect(watcher, &QDBusServiceWatcher::serviceRegistered, [&app](const QString &serviceName) {
        qDebug() << "Notification service registered:" << serviceName;
        int result = start();
        app.exit(result);
    });

    return app.exec();
}