#ifndef USERFRAME_H
#define USERFRAME_H

#include <QFrame>
#include <memory>
#include <QMap>

class UserButton;
class User;
class SessionBaseModel;
class UserFrame : public QFrame
{
    Q_OBJECT
public:
    explicit UserFrame(QWidget *parent = nullptr);
    void setModel(SessionBaseModel *model);

signals:
    void requestSwitchUser(std::shared_ptr<User> user);
    void hideFrame();

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

public slots:
    void userAdded(std::shared_ptr<User> user);
    void userRemoved(const uint uid);
    void expansion(bool expansion);
    void refreshPosition();
    void onUserClicked();

private:
    void switchNextUser();
    void switchPreviousUser();

private:
    QMap<UserButton*, uint> m_userBtns;
    SessionBaseModel *m_model;
    bool m_isExpansion;
};

#endif // USERFRAME_H
