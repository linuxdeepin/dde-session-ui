#include "userinfo.h"

const QString toLocalFile(const QString &path) {
    QUrl url(path);

    if (url.isLocalFile()) {
        return url.path();
    }

    return url.url();
}

User::User(QObject *parent)
    : QObject(parent)
    , m_isLogind(false)
    , m_isNoPasswdGrp(false)
{

}

User::User(const User &user)
    : m_isLogind(user.m_isLogind)
    , m_isNoPasswdGrp(user.m_isNoPasswdGrp)
    , m_uid(user.m_uid)
    , m_userName(user.m_userName)
    , m_locale(user.m_locale)
{

}

bool User::operator==(const User &user) const
{
    return type() == user.type() &&
            m_uid == user.m_uid;
}

void User::setLocale(const QString &locale)
{
    if (m_locale == locale) return;

    m_locale = locale;

    emit localeChanged(locale);
}

void User::setNoPasswdGrp(bool nopassword)
{
    m_isNoPasswdGrp = nopassword;
}

void User::setisLogind(bool isLogind) {
    if (m_isLogind == isLogind) {
        return;
    }

    m_isLogind = isLogind;

    emit logindChanged(isLogind);
}

NativeUser::NativeUser(const QString &path, QObject *parent)
    : User(parent)

    , m_userInter(new UserInter(ACCOUNT_DBUS_SERVICE, path, QDBusConnection::systemBus(), this))
{
    connect(m_userInter, &UserInter::IconFileChanged, this, &NativeUser::avatarChanged);
    connect(m_userInter, &UserInter::FullNameChanged, this, [=] (const QString &fullname) {
        emit displayNameChanged(fullname.isEmpty() ? m_userName : fullname);
    });

    connect(m_userInter, &UserInter::DesktopBackgroundsChanged, this, [=] {
        emit desktopBackgroundPathChanged(desktopBackgroundPath());
    });

    connect(m_userInter, &UserInter::GreeterBackgroundChanged, this, [=] (const QString &path) {
        emit greeterBackgroundPathChanged(toLocalFile(path));
    });

    connect(m_userInter, &UserInter::LocaleChanged, this, &NativeUser::setLocale);

    m_userName = m_userInter->userName();
    m_uid = m_userInter->uid().toInt();
    m_userPath = path;
    m_locale = m_userInter->locale();
}

void NativeUser::setCurrentLayout(const QString &layout)
{
    m_userInter->SetLayout(layout);
}

QString NativeUser::displayName() const
{
    const QString &fullname = m_userInter->fullName();
    return fullname.isEmpty() ? name() : fullname;
}

QString NativeUser::avatarPath() const
{
    return m_userInter->iconFile();
}

QString NativeUser::greeterBackgroundPath() const
{
    return toLocalFile(m_userInter->greeterBackground());
}

QString NativeUser::desktopBackgroundPath() const
{
    const QStringList &list = m_userInter->desktopBackgrounds();
    QString background;

    if (list.isEmpty()) {
        background = m_userInter->backgroundFile();
    } else {
        background = list.first();
    }

    return toLocalFile(background);
}

QStringList NativeUser::kbLayoutList()
{
    return m_userInter->historyLayout();
}

QString NativeUser::currentKBLayout()
{
    return m_userInter->layout();
}

ADDomainUser::ADDomainUser(int uid, QObject *parent)
    : User(parent)
{
    m_uid = uid;
}

void ADDomainUser::setUserDisplayName(const QString &name)
{
    if (m_userName == name) {
        return;
    }

    m_userName = name;

    emit displayNameChanged(name);
}

QString ADDomainUser::avatarPath() const
{
    return QString(":/img/default_avatar.png");
}

QString ADDomainUser::greeterBackgroundPath() const
{
    return QString("/usr/share/backgrounds/deepin/desktop.jpg");
}

QString ADDomainUser::desktopBackgroundPath() const
{
    return QString("/usr/share/backgrounds/deepin/desktop.jpg");
}
