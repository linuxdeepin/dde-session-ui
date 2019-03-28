#ifndef DEEPINAUTHINTERFACE_H
#define DEEPINAUTHINTERFACE_H

#include <QString>

class DeepinAuthInterface
{
public:
    virtual void onDisplayErrorMsg(const QString &msg) = 0;
    virtual void onDisplayTextInfo(const QString &msg) = 0;
    virtual void onPasswordResult(const QString &msg) = 0;
};

#endif // DEEPINAUTHINTERFACE_H
