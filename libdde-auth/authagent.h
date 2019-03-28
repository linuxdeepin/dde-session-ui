#ifndef AUTHAGENT_H
#define AUTHAGENT_H

#include "authority_interface.h"
#include "transaction_interface.h"

#include <QObject>

using Authority = com::deepin::daemon::Authority;
using Transaction = ComDeepinDaemonAuthorityTransactionInterface;

class DeepinAuthFramework;
class AuthAgent : public QObject {
    Q_OBJECT
public:
    enum Type {
        Keyboard,
        Fprint
    };

    explicit AuthAgent(Type type, QObject *parent = nullptr);
    ~AuthAgent();

    friend class AuthDBusAdaptor;

    void SetUser(const QString &username);
    void Authenticate();
    void Cancel();

public slots:
    const QString RequestEchoOff(const QString &msg);
    const QString RequestEchoOn(const QString &msg);
    void DisplayErrorMsg(const QString &msg);
    void DisplayTextInfo(const QString &msg);
    void RespondResult(const QString &msg);

    DeepinAuthFramework *parent();

private:
    Authority *m_authority;
    Transaction *m_transaction;
    Type m_type;
};

#endif // AUTHAGENT_H
