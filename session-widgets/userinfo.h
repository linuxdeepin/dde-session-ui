#ifndef USERINFO_H
#define USERINFO_H

#include <QObject>
#include <com_deepin_daemon_accounts_user.h>

#define ACCOUNT_DBUS_SERVICE "com.deepin.daemon.Accounts"
#define ACCOUNT_DBUS_PATH "/com/deepin/daemon/Accounts"

using UserInter = com::deepin::daemon::accounts::User;

class User : public QObject
{
    Q_OBJECT

public:
    enum UserType
    {
        Native,
        ADDomain,
    };

    User(QObject *parent);
    User(const User &user);

signals:
    void displayNameChanged(const QString &displayname) const;
    void logindChanged(bool islogind) const;
    void avatarChanged(const QString &avatar) const;
    void greeterBackgroundPathChanged(const QString &path) const;
    void desktopBackgroundPathChanged(const QString &path) const;
    void localeChanged(const QString &locale) const;

public:
    bool operator==(const User &user) const;
    const QString name() const { return m_userName; }
    bool isLogin() const { return m_isLogind; }
    uint uid() const { return m_uid; }

    const QString locale() const { return m_locale; }
    void setLocale(const QString &locale);

    bool isNoPasswdGrp() const { return m_isNoPasswdGrp; }
    void setNoPasswdGrp(bool nopassword);

    void setisLogind(bool isLogind);
    virtual void setCurrentLayout(const QString &layout) { Q_UNUSED(layout); }

    virtual UserType type() const = 0;
    virtual QString displayName() const { return m_userName; }
    virtual QString avatarPath() const = 0;
    virtual QString greeterBackgroundPath() const = 0;
    virtual QString desktopBackgroundPath() const = 0;
    virtual QStringList kbLayoutList() { return QStringList(); }
    virtual QString currentKBLayout() { return QString(); }

protected:
    bool m_isLogind;
    bool m_isNoPasswdGrp;
    uint m_uid;
    QString m_userName;
    QString m_locale;
};

class NativeUser : public User
{
    Q_OBJECT

public:
    NativeUser(const QString &path, QObject *parent = nullptr);

    void setCurrentLayout(const QString &currentKBLayout);

    UserType type() const { return Native; }
    QString displayName() const;
    QString avatarPath() const;
    QString greeterBackgroundPath() const;
    QString desktopBackgroundPath() const;
    QStringList kbLayoutList();
    QString currentKBLayout();

    const QString path() const { return m_userPath; }

private:
    UserInter *m_userInter;
    QString m_userPath;
};

class ADDomainUser : public User
{
    Q_OBJECT

public:
    ADDomainUser(int uid, QObject *parent = nullptr);

    void setUserDisplayName(const QString &name);

    UserType type() const { return ADDomain; }
    QString avatarPath() const;
    QString greeterBackgroundPath() const;
    QString desktopBackgroundPath() const;
};

#endif // USERINFO_H
