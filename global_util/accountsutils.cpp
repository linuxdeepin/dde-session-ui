#include "accountsutils.h"

#include <QProcess>
#include <QDebug>
#include <QSettings>
#include <QUrl>
#include <QRegularExpression>
#include <QLightDM/UsersModel>

using namespace QLightDM;

AccountsUtils::AccountsUtils()
{

}

QStringList AccountsUtils::GetLoggedInUsers()
{
    QStringList result;
    UsersModel model;

    const int userCount = model.rowCount(QModelIndex());
    for (int i(0); i != userCount; ++i)
    {
        const QString &username = model.data(model.index(i), UsersModel::NameRole).toString();
        const bool loggedIn = model.data(model.index(i), UsersModel::LoggedInRole).toBool();

        if (loggedIn) {
            result << username;
        }
    }

    qDebug() << "All logged in users: " << result;

    return result;
}

QString AccountsUtils::GetUserAvatar(QString &username)
{
    const QSettings settings("/var/lib/AccountsService/users/" + username, QSettings::IniFormat);
    const QString avatar = settings.value("User/Icon").toString();
    const QString path = QUrl(avatar).isLocalFile() ? QUrl(avatar).toLocalFile() : avatar;

    return path;
}
