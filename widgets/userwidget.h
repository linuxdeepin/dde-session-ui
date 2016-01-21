#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QKeyEvent>
#include <QFrame>

#include <QLightDM/UsersModel>

#include "userbutton.h"

#include <libdui/dloadingindicator.h>

DUI_USE_NAMESPACE

#define ACCOUNT_DBUS_SERVICE "com.deepin.daemon.Accounts"
#define ACCOUNT_DBUS_PATH "/com/deepin/daemon/Accounts"

class UserWidget : public QFrame
{
    Q_OBJECT
public:
    UserWidget(QWidget* parent = 0);
    ~UserWidget();

    const QString currentUser();
    inline int count() const {return m_userBtns->count();}
    bool isChooseUserMode = false;
signals:
    void userChanged(const QString &username);
    void chooseUserModeChanged(bool isChoose, QString curUser);
public slots:
    void addUser(QString avatar, QString name);
    void setCurrentUser(const QString &username);
    void expandWidget();
    void saveLastUser();
    inline void showLoadingAni() {m_loadingAni->show();}
    inline void hideLoadingAni() {m_loadingAni->hide();}

    void leftKeySwitchUser();
    void rightKeySwitchUser();
    void switchUserByKey(int i, int j);
    void chooseButtonChecked();

protected:
    void resizeEvent(QResizeEvent *e);
private slots:
    void initUI();
    void updateAvatar(QString username);
    QStringList getUsernameList();
private:
    int m_currentUserIndex = 0;
    int countNum = 0;
    QString m_currentUser = QString();
    QStringList whiteList;
    UserButton* m_currentBtns = nullptr;
    QList<UserButton *> *m_userBtns;
    QLightDM::UsersModel *m_userModel;
    DLoadingIndicator *m_loadingAni;

    const int USER_ICON_WIDTH = 140;
    const int USER_ICON_HEIGHT = 150;
};

#endif // WIDGET_H
