#ifndef LOCKCONTENT_H
#define LOCKCONTENT_H

#include <QWidget>
#include <memory>

#include "sessionbasewindow.h"

class ControlWidget;
class TimeWidget;
class UserInputWidget;
class SessionBaseModel;
class User;
class UserFrame;
class LockContent : public SessionBaseWindow
{
    Q_OBJECT
public:
    explicit LockContent(SessionBaseModel * const model, QWidget *parent = nullptr);

signals:
    void requestBackground(const QString &path);
    void requestAuthUser(std::shared_ptr<User> user, const QString &password);

public slots:
    void onCurrentUserChanged(std::shared_ptr<User> user);
    void pushUserFrame();

protected:
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void restoreCenterContent();

private:
    SessionBaseModel *m_model;
    ControlWidget *m_controlWidget;
    TimeWidget *m_timeWidget;
    UserInputWidget *m_userInputWidget;
    UserFrame *m_userFrame;
    std::shared_ptr<User> m_user;
};

#endif // LOCKCONTENT_H
