#ifndef ACCOUNTSUTILS_H
#define ACCOUNTSUTILS_H

#include <QStringList>

class AccountsUtils
{
public:
    AccountsUtils();

    static QStringList GetLoggedInUsers();
    static QString GetUserAvatar(QString & username);
};

#endif // ACCOUNTSUTILS_H
