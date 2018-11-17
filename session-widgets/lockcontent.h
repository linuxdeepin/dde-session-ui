#ifndef LOCKCONTENT_H
#define LOCKCONTENT_H

#include <QWidget>
#include <memory>

#include "sessionbasewindow.h"

class ControlWidget;
class UserInputWidget;
class SessionBaseModel;
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

public slots:
    void onCurrentUserChanged(std::shared_ptr<User> user);
    void pushUserFrame();
    void pushSessionFrame();
    void pushShutdownFrame();

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void restoreCenterContent();

private:
    SessionBaseModel *m_model;
    ControlWidget *m_controlWidget;
    UserInputWidget *m_userInputWidget;
    UserFrame *m_userFrame;
    SessionWidget *m_sessionFrame;
    ShutdownWidget *m_shutdownFrame;

    std::shared_ptr<User> m_user;
};

#endif // LOCKCONTENT_H
