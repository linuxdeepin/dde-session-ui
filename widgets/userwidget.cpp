#include "userwidget.h"
#include "constants.h"
#include "dbus/dbusaccounts.h"
#include "dbus/dbususer.h"

#include <QApplication>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include <QSettings>

#include <unistd.h>

UserWidget::UserWidget(QWidget* parent)
    : QFrame(parent),
    m_userBtns(new QList<UserButton *>),
    m_userModel(new QLightDM::UsersModel(this))
{
    QSettings settings(DDESESSIONCC::CONFIG_FILE, QSettings::IniFormat);
    m_currentUser = settings.value("last-user").toString();

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    setStyleSheet("background-color:red;");
    setFixedSize(qApp->desktop()->width(), USER_ICON_HEIGHT);
//    move(0, (qApp->desktop()->height() - rect().height()) / 2 - 95);

    initUI();
}

UserWidget::~UserWidget()
{

}

void UserWidget::initUI()
{
    DBusAccounts *accounts = new DBusAccounts("com.deepin.daemon.Accounts", "/com/deepin/daemon/Accounts", QDBusConnection::systemBus(), this);
    const QStringList userList = accounts->userList();
    QStringList whiteList;

    for (const QString &user : userList)
    {
        DBusUser *inter = new DBusUser("com.deepin.daemon.Accounts", user, QDBusConnection::systemBus(), this);

        if (!inter->locked())
            whiteList.append(inter->userName());
        inter->deleteLater();
    }
    accounts->deleteLater();

    qDebug() << "whiteList:             " << whiteList;

    const int userCount = m_userModel->rowCount(QModelIndex());
    for (int i(0); i != userCount; ++i)
    {
        const QString &username = m_userModel->data(m_userModel->index(i), QLightDM::UsersModel::NameRole).toString();

        // pass blocked account
        if (!whiteList.contains(username))
            continue;

        const QSettings settings("/var/lib/AccountsService/users/" + username, QSettings::IniFormat);

        addUser(settings.value("User/Icon").toString(), username);
    }

    QPixmap loading(":/img/facelogin_animation.png");
    QSize size(110, 110);
    m_loadingAni = new DLoadingIndicator(this);
    m_loadingAni->setLoading(true);
    m_loadingAni->setSmooth(true);
    m_loadingAni->setAniDuration(600);
    m_loadingAni->setAniEasingCurve(QEasingCurve::Linear);
    m_loadingAni->move(rect().center().x() - UserButton::AvatarLargerSize / 2,
                       rect().center().y() - UserButton::AvatarLargerSize / 2 - 14);
    m_loadingAni->setFixedSize(size);
    m_loadingAni->setImageSource(loading.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    m_loadingAni->hide();

    setCurrentUser(currentUser());
}

void UserWidget::setCurrentUser(const QString &username)
{
    isChooseUserMode = false;
    for (UserButton *user : *m_userBtns) {
        if (user->objectName() == username) {
            user->showButton();
            user->setImageSize(user->AvatarLargerSize);
            if (user->isChecked())
                user->setButtonChecked(false);
        } else
            user->hide(180);

        user->move(rect().center() - user->rect().center(), 200);
    }

    m_currentUser = username;

    emit userChanged(username);
}

void UserWidget::addUser(QString avatar, QString name) {
    UserButton *user = new UserButton(avatar, name);
    user->hide();
    user->setObjectName(name);
    user->setParent(this);
    user->move(rect().center() - user->rect().center());

    connect(user, &UserButton::imageClicked, this, &UserWidget::setCurrentUser);

    m_userBtns->append(user);
}

void UserWidget::expandWidget()
{
    isChooseUserMode = true;
    const int count = m_userBtns->count();
    const int offset = (width() - USER_ICON_WIDTH * count) / 2;

    for (int i = 0; i != count; ++i)
    {
        if (m_userBtns->at(i)->objectName() != currentUser()) {
             m_userBtns->at(i)->showButton();
        }

        m_userBtns->at(i)->stopAnimation();

        m_userBtns->at(i)->show();
        m_userBtns->at(i)->setImageSize(UserButton::AvatarSmallSize);
        m_userBtns->at(i)->move(QPoint(offset + i * USER_ICON_WIDTH, 0), 200);

    }
}

void UserWidget::saveLastUser()
{
    QSettings settings(DDESESSIONCC::CONFIG_FILE, QSettings::IniFormat);
    settings.setValue("last-user", currentUser());
}

void UserWidget::resizeEvent(QResizeEvent *e)
{
    QFrame::resizeEvent(e);

    for (UserButton *user : *m_userBtns)
        user->move(rect().center() - user->rect().center(), 1);

    m_loadingAni->move(rect().center().x() - UserButton::AvatarLargerSize / 2,
                       rect().center().y() - UserButton::AvatarLargerSize / 2 - 14);
}

void UserWidget::switchUserByKey(int i, int j) {
    m_userBtns->at(i)->hide(10);
    m_currentUser = m_userBtns->at(j)->objectName();
    m_userBtns->at(j)->show();
    m_userBtns->at(j)->showButton();
    m_userBtns->at(j)->setImageSize(UserButton::AvatarLargerSize);
}

void UserWidget::chooseButtonChecked() {
    qDebug() << "Grab Key Release Event";

    this->grabKeyboard();
    qDebug() << "Get the Key Return or Enter";
    bool checkedBtsExist = false;
    for (UserButton* user: *m_userBtns) {
        if (user->isChecked()) {
            setCurrentUser(user->objectName());
            checkedBtsExist = true;
        }
    }
    if (!checkedBtsExist) {
        setCurrentUser(m_currentUser);
    }
}

void UserWidget::leftKeySwitchUser() {

    if (!isChooseUserMode) {
        for (int i = 0; i < m_userBtns->length(); i++) {
            qDebug() << "zz:" << i << m_userBtns->at(i)->objectName();
            if (m_userBtns->at(i)->objectName() == m_currentUser) {

                if (i == 0) {
                    switchUserByKey(0, m_userBtns->length() - 1);
                    break;
                } else {
                    qDebug() << "$$$" << i;
                    switchUserByKey(i, i - 1);
                    break;
                }
            } else {
                continue;
            }

        }
    } else {
        if (m_currentUserIndex == 0) {
            m_currentUserIndex = m_userBtns->length() - 1;
        } else {
            m_currentUserIndex = m_currentUserIndex - 1;
        }

        for (int j = 0; j < m_userBtns->length(); j++) {
            if (j == m_currentUserIndex) {
                m_userBtns->at(j)->setButtonChecked(true);
            } else {
                m_userBtns->at(j)->setButtonChecked(false);
            }
        }

    }
}

void UserWidget::rightKeySwitchUser() {
    qDebug() << "RightKeyPressed";
    if (!isChooseUserMode) {
        for (int i = 0; i < m_userBtns->length(); i++) {
            if (m_userBtns->at(i)->objectName() == m_currentUser) {

                if (i == m_userBtns->length() - 1) {
                    switchUserByKey(m_userBtns->length() - 1, 0);
                    break;
                } else {
                    qDebug() << "$$$" << i;
                    switchUserByKey(i, i + 1);
                    break;
                }
            } else {
                continue;
            }
        }
    } else {
        if (m_currentUserIndex ==  m_userBtns->length() - 1) {
            m_currentUserIndex = 0;
        } else {
            m_currentUserIndex = m_currentUserIndex + 1;
        }

        for (int j = 0; j < m_userBtns->length(); j++) {
            if (j == m_currentUserIndex) {
                m_userBtns->at(j)->setButtonChecked(true);
            } else {
                m_userBtns->at(j)->setButtonChecked(false);
            }
        }
    }
}

const QString UserWidget::currentUser() const
{
    qDebug() << "currentUser:" << m_currentUser;

    if (!m_currentUser.isEmpty())
        return m_currentUser;

    const QString currentLogin = getlogin();
    if (!currentLogin.isEmpty())
        return currentLogin;

    // return first user
    if (m_userModel->rowCount(QModelIndex()))
        return m_userModel->data(m_userModel->index(0), QLightDM::UsersModel::NameRole).toString();

    qWarning() << "no users !!!";
    return QString();
}
