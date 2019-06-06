#ifndef USERINPUTWIDGET_H
#define USERINPUTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <DPasswdEditAnimated>
#include <QPushButton>
#include <functional>
#include <darrowrectangle.h>

#include "useravatar.h"
#include "framedatabind.h"
#include "userinfo.h"
#include "otheruserinput.h"
#include "lockpasswordwidget.h"
#include "sessionbasemodel.h"
#include "loginbutton.h"

#include <memory>

DWIDGET_USE_NAMESPACE

class KbLayoutWidget;
class UserInputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserInputWidget(QWidget *parent = nullptr);
    ~UserInputWidget();

    void setUser(std::shared_ptr<User> user);

    void setIsNoPasswordGrp(bool isNopassword);
    void setFaildMessage(const QString &message);
    void setFaildTipMessage(const QString &message);
    void updateKBLayout(const QStringList &list);
    void setDefaultKBLayout(const QString &layout);
    void disablePassword(bool disable);
    void updateAuthType(SessionBaseModel::AuthType type);

    void resetAllState();

    void shutdownMode();
    void normalMode();
    void restartMode();

    void grabKeyboard();
    void releaseKeyboard();

    void hideKeyboard();

signals:
    void requestAuthUser(const QString &password);
    void abortOperation();
    void requestUserKBLayoutChanged(const QString &layout);

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject* watched, QEvent* event) override;

private:
    void setName(const QString &name);
    void setAvatar(const QString &avatar);
    void refreshLanguage();
    void refreshAvatarPosition();
    void toggleKBLayoutWidget();
    void refreshKBLayoutWidgetPosition();
    void refreshInputState();
    void onOtherPagePasswordChanged(const QVariant &value);
    void onOtherPageKBLayoutChanged(const QVariant &value);

private:
    UserAvatar *m_userAvatar;
    QLabel *m_nameLbl;
    DPasswdEditAnimated *m_passwordEdit;
    OtherUserInput *m_otherUserInput;
    LoginButton *m_loginBtn;
    DArrowRectangle *m_kbLayoutBorder;
    KbLayoutWidget *m_kbLayoutWidget;
    LockPasswordWidget *m_lockPasswordWidget;
    std::shared_ptr<User> m_user;
    QList<QMetaObject::Connection> m_currentUserConnects;
    std::list<std::pair<std::function<void (QString)>, QString>> m_trList;
    SessionBaseModel::AuthType m_authType;
    QMap<uint, QString> m_passwords;
};

#endif // USERINPUTWIDGET_H
