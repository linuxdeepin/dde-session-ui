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
class LockContent : public SessionBaseWindow
{
    Q_OBJECT
public:
    explicit LockContent(SessionBaseModel * const model, QWidget *parent = nullptr);

signals:
    void requestBackground(const QString &path);

public slots:
    void onCurrentUserChanged(std::shared_ptr<User> user);

private:
    SessionBaseModel *m_model;
    ControlWidget *m_controlWidget;
    TimeWidget *m_timeWidget;
    UserInputWidget *m_userInputWidget;
};

#endif // LOCKCONTENT_H
