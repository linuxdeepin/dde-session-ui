#ifndef LOCKCONTENT_H
#define LOCKCONTENT_H

#include <QWidget>
#include <memory>

#include <com_deepin_daemon_imageblur.h>

#include "sessionbasewindow.h"
#include "sessionbasemodel.h"

using ImageBlur = com::deepin::daemon::ImageBlur;

class ControlWidget;
class UserInputWidget;
class User;
class UserFrame;
class SessionWidget;
class ShutdownWidget;
class LockContent : public SessionBaseWindow
{
    Q_OBJECT
public:
    explicit LockContent(SessionBaseModel * const model, QWidget *parent = nullptr);

signals:
    void requestBackground(const QString &path);
    void requestAuthUser(std::shared_ptr<User> user, const QString &password);
    void requestSwitchToUser(std::shared_ptr<User> user);
    void requestSetLayout(std::shared_ptr<User> user, const QString &value);

public slots:
    void onCurrentUserChanged(std::shared_ptr<User> user);
    void pushUserFrame();
    void pushSessionFrame();
    void pushShutdownFrame();
    void onStatusChanged(SessionBaseModel::ModeStatus status);

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    void releaseAllKeyboard();
    void restoreCenterContent();
    void restoreMode();
    void updateBackground(const QString &path);
    void onBlurDone(const QString &source, const QString &blur, bool status);
    void toggleVirtualKB();
    void updateVirtualKBPosition();

private:
    SessionBaseModel *m_model;
    ControlWidget *m_controlWidget;
    UserInputWidget *m_userInputWidget;
    UserFrame *m_userFrame;
    SessionWidget *m_sessionFrame;
    ShutdownWidget *m_shutdownFrame;
    ImageBlur *m_imageBlurInter;
    QWidget *m_virtualKB;
    std::shared_ptr<User> m_user;
    QList<QMetaObject::Connection> m_currentUserConnects;
    QTranslator *m_translator;
};

#endif // LOCKCONTENT_H
