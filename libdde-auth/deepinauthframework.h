#ifndef DEEPINAUTHFRAMEWORK_H
#define DEEPINAUTHFRAMEWORK_H

#include "authagent.h"

#include <QObject>
#include <QPointer>
#include <memory>

class DeepinAuthInterface;
class User;
class DeepinAuthFramework : public QObject
{
    Q_OBJECT
public:
    explicit DeepinAuthFramework(DeepinAuthInterface *inter, QObject *parent = nullptr);
    ~DeepinAuthFramework();

    friend class AuthAgent;

    bool isAuthenticate() const;

public slots:
    void SetUser(std::shared_ptr<User> user);
    void Authenticate();
    void Clear();
    void setPassword(const QString &password);

private:
    const QString RequestEchoOff(const QString &msg);
    const QString RequestEchoOn(const QString &msg);
    void DisplayErrorMsg(AuthAgent::Type type, const QString &msg);
    void DisplayTextInfo(AuthAgent::Type type, const QString &msg);
    void RespondResult(AuthAgent::Type type, const QString &msg);

private:
    DeepinAuthInterface *m_interface;
    QPointer<AuthAgent> m_keyboard;
    QPointer<AuthAgent> m_fprint;
};

#endif // DEEPINAUTHFRAMEWORK_H
