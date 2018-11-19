#ifndef USERINPUTWIDGET_H
#define USERINPUTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <DPasswdEditAnimated>
#include <QPushButton>
#include <functional>

#include "useravatar.h"

DWIDGET_USE_NAMESPACE

class UserInputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserInputWidget(QWidget *parent = nullptr);

    void setAvatar(const QString &avatar);
    void setIsNoPasswordGrp(bool isNopassword);
    void setFaildMessage(const QString &message);
    void setFaildTipMessage(const QString &message);

    void shutdownMode();
    void normalMode();
    void restartMode();

    void grabKeyboard();
    void releaseKeyboard();

signals:
    void requestAuthUser(const QString &password);

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    void refreshLanguage();

private:
    UserAvatar *m_userAvatar;
    DPasswdEditAnimated *m_passwordEdit;
    QPushButton *m_loginBtn;
    std::list<std::pair<std::function<void (QString)>, QString>> m_trList;
};

#endif // USERINPUTWIDGET_H
