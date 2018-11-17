#include "lockcontent.h"

#include "controlwidget.h"
#include "timewidget.h"
#include "userinputwidget.h"
#include "sessionbasemodel.h"

LockContent::LockContent(SessionBaseModel * const model, QWidget *parent)
    : SessionBaseWindow(parent)
    , m_model(model)
{
    m_timeWidget = new TimeWidget;
    m_controlWidget = new ControlWidget;
    m_userInputWidget = new UserInputWidget;

#ifdef QT_DEBUG
    m_userInputWidget->setAvatar("/var/lib/AccountsService/icons/1.png");
#endif

    setCenterContent(m_userInputWidget);
    setLeftBottomWidget(m_timeWidget);
    setRightBottomWidget(m_controlWidget);

    switch (model->currentType()) {
    case SessionBaseModel::AuthType::LightdmType:
        m_controlWidget->setSessionSwitchEnable(true);
        break;
    case SessionBaseModel::AuthType::LockType:
        m_controlWidget->setMPRISEnable(true);
        break;
    default:
        break;
    }

    // init connect
    connect(model, &SessionBaseModel::currentUserChanged, this, &LockContent::onCurrentUserChanged);
    connect(m_userInputWidget, &UserInputWidget::requestAuthUser, this, [=] (const QString &password) {
        emit requestAuthUser(m_user, password);
    });
}

void LockContent::onCurrentUserChanged(std::shared_ptr<User> user)
{
    m_user = user;

    connect(user.get(), &User::greeterBackgroundPathChanged, this, &LockContent::requestBackground, Qt::UniqueConnection);

    m_userInputWidget->setAvatar(user->avatarPath());
    m_userInputWidget->setIsNoPasswordGrp(user->isNoPasswdGrp());

    //TODO: refresh blur image
    emit requestBackground(user->greeterBackgroundPath());
}
