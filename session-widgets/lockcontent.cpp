#include "lockcontent.h"

#include "controlwidget.h"
#include "userinputwidget.h"
#include "sessionbasemodel.h"
#include "userframe.h"
#include "sessionwidget.h"
#include "shutdownwidget.h"

#include <QMouseEvent>

LockContent::LockContent(SessionBaseModel * const model, QWidget *parent)
    : SessionBaseWindow(parent)
    , m_model(model)
{
    m_controlWidget = new ControlWidget;
    m_userInputWidget = new UserInputWidget;
    m_userFrame = new UserFrame;
    m_shutdownFrame = new ShutdownWidget;

    m_shutdownFrame->setModel(model);
    m_userFrame->setModel(model);

    setCenterContent(m_userInputWidget);
    setRightBottomWidget(m_controlWidget);

    switch (model->currentType()) {
    case SessionBaseModel::AuthType::LightdmType:
        m_controlWidget->setSessionSwitchEnable(true);
        m_sessionFrame = new SessionWidget;
        m_sessionFrame->setModel(model);

        connect(m_sessionFrame, &SessionWidget::hideFrame, this, &LockContent::restoreCenterContent);
        connect(m_controlWidget, &ControlWidget::requestSwitchSession, this, &LockContent::pushSessionFrame);
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
    connect(m_userFrame, &UserFrame::requestSwitchUser, this, &LockContent::requestSwitchToUser);
    connect(m_controlWidget, &ControlWidget::requestSwitchUser, this, &LockContent::pushUserFrame);
    connect(m_controlWidget, &ControlWidget::requestShutdown, this, &LockContent::pushShutdownFrame);
    connect(m_userFrame, &UserFrame::hideFrame, this, &LockContent::restoreCenterContent);
    connect(m_shutdownFrame, &ShutdownWidget::abortOperation, this, &LockContent::restoreCenterContent);
    connect(model, &SessionBaseModel::authFaildMessage, m_userInputWidget, &UserInputWidget::setFaildMessage);
    connect(model, &SessionBaseModel::authFaildTipsMessage, m_userInputWidget, &UserInputWidget::setFaildTipMessage);

    connect(model, &SessionBaseModel::onPowerActionChanged, this, [=] (SessionBaseModel::PowerAction poweraction) {
        switch (poweraction) {
        case SessionBaseModel::RequireNormal:
            m_userInputWidget->normalMode();
            break;
        case SessionBaseModel::RequireRestart:
            m_userInputWidget->restartMode();
            break;
        case SessionBaseModel::RequireShutdown:
            m_userInputWidget->shutdownMode();
        default:
            break;
        }
    });
}

void LockContent::onCurrentUserChanged(std::shared_ptr<User> user)
{
    m_user = user;

    connect(user.get(), &User::greeterBackgroundPathChanged, this, &LockContent::requestBackground, Qt::UniqueConnection);

    m_userInputWidget->setAvatar(user->avatarPath());
    m_userInputWidget->setIsNoPasswordGrp(user->isNoPasswdGrp());

    if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
        m_sessionFrame->switchToUser(user->name());
    }

    //TODO: refresh blur image
    emit requestBackground(user->greeterBackgroundPath());
}

void LockContent::pushUserFrame()
{
    setCenterContent(m_userFrame);
    m_userFrame->expansion(true);
}

void LockContent::pushSessionFrame()
{
    setCenterContent(m_sessionFrame);
}

void LockContent::pushShutdownFrame()
{
    setCenterContent(m_shutdownFrame);
}

void LockContent::mouseReleaseEvent(QMouseEvent *event)
{
    restoreCenterContent();

    return SessionBaseWindow::mouseReleaseEvent(event);
}

void LockContent::restoreCenterContent()
{
    setCenterContent(m_userInputWidget);
}
