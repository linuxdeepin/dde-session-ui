#include "userwidget.h"
#include <QApplication>
#include <QtWidgets>
#include <QtGui>
#include <QtCore>
#include <QSettings>

UserWidget::UserWidget(QWidget* parent)
    : QFrame(parent),
    m_userBtns(new QList<UserButton *>),
    m_userModel(new QLightDM::UsersModel(this))
{
    QSettings settings("/var/lib/lightdm/qt-lightdm-greeter/state", QSettings::IniFormat);
    m_currentUser = settings.value("last-user").toString();

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
//    setStyleSheet("background-color:red;");
    setFixedSize(qApp->desktop()->width(), USER_ICON_HEIGHT);
    move(0, (qApp->desktop()->height() - rect().height()) / 2 - 95);

    initUI();
}

UserWidget::~UserWidget()
{

}

void UserWidget::initUI() {
    const int userCount = m_userModel->rowCount(QModelIndex());
    for (int i(0); i != userCount; ++i)
    {
        const QString &username = m_userModel->data(m_userModel->index(i), QLightDM::UsersModel::NameRole).toString();
        const QSettings settings("/var/lib/AccountsService/users/" + username, QSettings::IniFormat);

        addUser(settings.value("User/Icon").toString(), username);
    }

    setCurrentUser(currentUser());
}

void UserWidget::setCurrentUser(const QString &username) {

    for (UserButton *user : *m_userBtns) {
        if (user->objectName() == username) {
            user->show();
            user->showButton();
            user->setImageSize(user->AvatarLargerSize);
        } else
            user->hide(180);

         user->move(rect().center() - user->rect().center(), 200);
//        user->setGeometry(QRect(rect().center()- user->rect().center(), 200, user->width(), user->height()));

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

void UserWidget::expandWidget() {


    const int count = m_userBtns->count();
    const int offset = (width() - USER_ICON_WIDTH * count) / 2;

    for (int i = 0; i != count; ++i)
    {
        m_userBtns->at(i)->showButton();
        m_userBtns->at(i)->setImageSize(UserButton::AvatarSmallSize);
        m_userBtns->at(i)->move(QPoint(offset + i * USER_ICON_WIDTH, 0), 200);
    }
}

const QString UserWidget::currentUser() const
{
    qDebug() << m_currentUser;

    if (!m_currentUser.isEmpty())
        return m_currentUser;

    // return first user
    if (m_userModel->rowCount(QModelIndex()))
        return m_userModel->data(m_userModel->index(0), QLightDM::UsersModel::NameRole).toString();

    qWarning() << "no users !!!";
    return QString();
}
