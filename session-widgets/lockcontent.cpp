#include "lockcontent.h"

#include "controlwidget.h"
#include "userinputwidget.h"
#include "sessionbasemodel.h"
#include "userframe.h"
#include "sessionwidget.h"
#include "shutdownwidget.h"
#include "virtualkbinstance.h"

#include <QMouseEvent>

LockContent::LockContent(SessionBaseModel * const model, QWidget *parent)
    : SessionBaseWindow(parent)
    , m_model(model)
    , m_imageBlurInter(new ImageBlur("com.deepin.daemon.Accounts", "/com/deepin/daemon/ImageBlur", QDBusConnection::systemBus(), this))
    , m_virtualKB(nullptr)
    , m_translator(new QTranslator)
{
    m_controlWidget = new ControlWidget;
    m_userInputWidget = new UserInputWidget;
    m_userFrame = new UserFrame;
    m_shutdownFrame = new ShutdownWidget;

    m_userInputWidget->updateAuthType(model->currentType());

    m_shutdownFrame->setModel(model);
    m_userFrame->setModel(model);

    model->setCurrentModeState(SessionBaseModel::ModeStatus::PasswordMode);

    setRightBottomWidget(m_controlWidget);

    switch (model->currentType()) {
    case SessionBaseModel::AuthType::LightdmType:
        m_controlWidget->setSessionSwitchEnable(true);
        m_sessionFrame = new SessionWidget;
        m_sessionFrame->setModel(model);

        connect(m_sessionFrame, &SessionWidget::hideFrame, this, &LockContent::restoreMode);
        connect(m_sessionFrame, &SessionWidget::sessionChanged, this, &LockContent::restoreMode);
        connect(m_controlWidget, &ControlWidget::requestSwitchSession, this, [=] {
            m_model->setCurrentModeState(SessionBaseModel::ModeStatus::SessionMode);
        });
        connect(m_model, &SessionBaseModel::onSessionKeyChanged, m_controlWidget, &ControlWidget::chooseToSession);
        connect(m_model, &SessionBaseModel::onSessionKeyChanged, this, &LockContent::restoreMode);
        break;
    case SessionBaseModel::AuthType::LockType:
        m_controlWidget->setMPRISEnable(true);
        break;
    default:
        break;
    }

    // init connect
    connect(model, &SessionBaseModel::currentUserChanged, this, &LockContent::onCurrentUserChanged);
    connect(m_userInputWidget, &UserInputWidget::requestAuthUser, this, &LockContent::requestAuthUser);
    connect(m_userFrame, &UserFrame::requestSwitchUser, this, &LockContent::requestSwitchToUser);
    connect(m_userFrame, &UserFrame::requestSwitchUser, this, &LockContent::restoreMode);
    connect(m_controlWidget, &ControlWidget::requestSwitchUser, this, [=] {
        m_model->setCurrentModeState(SessionBaseModel::ModeStatus::UserMode);
    });
    connect(m_controlWidget, &ControlWidget::requestShutdown, this, [=] {
        m_model->setCurrentModeState(SessionBaseModel::ModeStatus::PowerMode);
    });
    connect(m_controlWidget, &ControlWidget::requestSwitchVirtualKB, this, &LockContent::toggleVirtualKB);
    connect(model, &SessionBaseModel::hasVirtualKBChanged, m_controlWidget, &ControlWidget::setVirtualKBVisible);
    connect(m_userFrame, &UserFrame::hideFrame, this, &LockContent::restoreMode);
    connect(m_shutdownFrame, &ShutdownWidget::abortOperation, this, &LockContent::restoreMode);
    connect(model, &SessionBaseModel::authFaildMessage, m_userInputWidget, &UserInputWidget::setFaildMessage);
    connect(model, &SessionBaseModel::authFaildTipsMessage, m_userInputWidget, &UserInputWidget::setFaildTipMessage);
    connect(model, &SessionBaseModel::onStatusChanged, this, &LockContent::onStatusChanged);
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

    connect(m_userInputWidget, &UserInputWidget::abortOperation, this, [=] {
        model->setPowerAction(SessionBaseModel::PowerAction::RequireNormal);
    });

    connect(m_userInputWidget, &UserInputWidget::requestUserKBLayoutChanged, this, [=] (const QString &value) {
        emit requestSetLayout(m_user, value);
    });

    auto initVirtualKB = [=] (bool hasvirtualkb) {
        if (hasvirtualkb && !m_virtualKB) {
            VirtualKBInstance::Instance();
            QTimer::singleShot(500, this, [=] {
                m_virtualKB = VirtualKBInstance::Instance().virtualKBWidget();
            });
        }
    };

    connect(model, &SessionBaseModel::hasVirtualKBChanged, this, initVirtualKB);

    onCurrentUserChanged(model->currentUser());
    m_controlWidget->setVirtualKBVisible(model->hasVirtualKB());
    initVirtualKB(model->hasVirtualKB());
}

void LockContent::onCurrentUserChanged(std::shared_ptr<User> user)
{
    if (user.get() == nullptr) return; // if dbus is async

    // set user language
    qApp->removeTranslator(m_translator);
    const QString locale { user->locale() };
    m_translator->load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale(locale.isEmpty() ? "en_US.UTF8" : locale).name());
    qApp->installTranslator(m_translator);

    for (auto connect : m_currentUserConnects) {
        m_user.get()->disconnect(connect);
    }

    m_currentUserConnects.clear();

    m_user = user;

    m_currentUserConnects << connect(user.get(), &User::greeterBackgroundPathChanged, this, &LockContent::updateBackground , Qt::UniqueConnection);

    m_userInputWidget->setUser(user);

    if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
        m_sessionFrame->switchToUser(user->name());
    }

    //TODO: refresh blur image
    QTimer::singleShot(0, this, [=] {
        updateBackground(user->greeterBackgroundPath());
    });
}

void LockContent::pushUserFrame()
{
    releaseAllKeyboard();
    setCenterContent(m_userFrame);
    m_userFrame->expansion(true);

    QTimer::singleShot(300, m_userFrame, &UserFrame::grabKeyboard);
}

void LockContent::pushSessionFrame()
{
    releaseAllKeyboard();
    setCenterContent(m_sessionFrame);
    m_sessionFrame->show();

    QTimer::singleShot(300, m_sessionFrame, &SessionWidget::grabKeyboard);
}

void LockContent::pushShutdownFrame()
{
    releaseAllKeyboard();
    setCenterContent(m_shutdownFrame);

    QTimer::singleShot(300, m_shutdownFrame, &ShutdownWidget::grabKeyboard);
}

void LockContent::onStatusChanged(SessionBaseModel::ModeStatus status)
{
    switch (status) {
    case SessionBaseModel::ModeStatus::PasswordMode:
        restoreCenterContent();
        break;
    case SessionBaseModel::ModeStatus::UserMode:
        pushUserFrame();
        break;
    case SessionBaseModel::ModeStatus::SessionMode:
        pushSessionFrame();
        break;
    case SessionBaseModel::ModeStatus::PowerMode:
        pushShutdownFrame();
        break;
    default:
        break;
    }
}

void LockContent::mouseReleaseEvent(QMouseEvent *event)
{
    m_model->setCurrentModeState(SessionBaseModel::ModeStatus::PasswordMode);

    // hide keyboardlayout widget
    m_userInputWidget->hideKeyboard();

    return SessionBaseWindow::mouseReleaseEvent(event);
}

void LockContent::showEvent(QShowEvent *event)
{
    onStatusChanged(m_model->currentModeState());

    return QFrame::showEvent(event);
}

void LockContent::hideEvent(QHideEvent *event)
{
    releaseAllKeyboard();

    return QFrame::hideEvent(event);
}

void LockContent::resizeEvent(QResizeEvent *event)
{
    QTimer::singleShot(0, this, [=] {
        if (m_virtualKB && m_virtualKB->isVisible()) {
            updateVirtualKBPosition();
        }
    });

    return QFrame::resizeEvent(event);
}

void LockContent::releaseAllKeyboard()
{
    m_userInputWidget->releaseKeyboard();
    m_userFrame->releaseKeyboard();
    m_shutdownFrame->releaseKeyboard();

    if (m_model->currentType() == SessionBaseModel::AuthType::LightdmType) {
        m_sessionFrame->releaseKeyboard();
    }
}

void LockContent::restoreCenterContent()
{
    releaseAllKeyboard();
    setCenterContent(m_userInputWidget);

    QTimer::singleShot(300, m_userInputWidget, &UserInputWidget::grabKeyboard);
}

void LockContent::restoreMode()
{
    m_model->setCurrentModeState(SessionBaseModel::ModeStatus::PasswordMode);
}

void LockContent::updateBackground(const QString &path)
{
    const QString &wallpaper = m_imageBlurInter->Get(path);

    emit requestBackground(wallpaper.isEmpty() ? path : wallpaper);
}

void LockContent::onBlurDone(const QString &source, const QString &blur, bool status)
{
    const QString &sourcePath = QUrl(source).isLocalFile() ? QUrl(source).toLocalFile() : source;

    if (status && m_model->currentUser()->greeterBackgroundPath() == sourcePath)
        emit requestBackground(blur);
}

void LockContent::toggleVirtualKB()
{
    m_virtualKB->setParent(this);
    m_virtualKB->setVisible(!m_virtualKB->isVisible());
    m_virtualKB->raise();

    updateVirtualKBPosition();
}

void LockContent::updateVirtualKBPosition()
{
    const QPoint point = geometry().topLeft() + mapToGlobal(QPoint((width() - m_virtualKB->width()) / 2, height() - m_virtualKB->height() - 50));
    m_virtualKB->move(point);
}