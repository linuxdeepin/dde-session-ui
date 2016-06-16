#ifndef MULTIUSERSWARNINGVIEW_H
#define MULTIUSERSWARNINGVIEW_H

#include <QFrame>

#include <dimagebutton.h>

#include "common.h"

class QListWidget;
class QLabel;
class QVBoxLayout;

DWIDGET_USE_NAMESPACE

class MultiUsersWarningView : public QFrame
{
    Q_OBJECT
public:
    MultiUsersWarningView(QWidget *parent = 0);

    void setUsers(QStringList &users);

    Actions action() const;
    void setAction(const Actions action);

signals:
    void actionInvoked();
    void cancelled();

private:
    QVBoxLayout * m_vLayout;
    QListWidget * m_userList;
    QLabel * m_warningTip;
    DImageButton * m_cancelBtn;
    DImageButton * m_actionBtn;

    Actions m_action;
};

class UserListItem : public QFrame
{
    Q_OBJECT
public:
    UserListItem(QString &icon, QString &name);

private:
    QLabel * m_icon;
    QLabel * m_name;

    QPixmap getRoundPixmap(QString & path);
};

#endif // MULTIUSERSWARNINGVIEW_H
