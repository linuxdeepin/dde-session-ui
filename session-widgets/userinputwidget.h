#ifndef USERINPUTWIDGET_H
#define USERINPUTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <DPasswdEditAnimated>
#include "useravatar.h"

DWIDGET_USE_NAMESPACE

class UserInputWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserInputWidget(QWidget *parent = nullptr);

    void setAvatar(const QString &avatar);

private:
    UserAvatar *m_userAvatar;
    DPasswdEditAnimated *m_passwordEdit;
};

#endif // USERINPUTWIDGET_H
