#ifndef LoginManager_H
#define LoginManager_H
#include <QApplication>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

#include "view/logowidget.h"
#include "view/switchframe.h"

#include "backgroundlabel.h"
#include "util_updateui.h"
#include "passwdedit.h"
#include "userwidget.h"
#include "sessionwidget.h"

#include <QLightDM/Greeter>
#include <QLightDM/SessionsModel>

#include "view/logowidget.h"
#include "view/switchframe.h"

#include "backgroundlabel.h"
#include "util_updateui.h"
#include "passwdedit.h"
#include "userwidget.h"
#include "sessionwidget.h"

#include <QLightDM/Greeter>
#include <QLightDM/SessionsModel>

class BackgroundLabel;
class LoginManager: public QFrame {
    Q_OBJECT
public:
    LoginManager(QWidget* parent=0);
    ~LoginManager();

protected:
    void keyPressEvent(QKeyEvent* e);

private slots:
    void initUI();
    void initConnect();
    void login();
    void prompt(QString text, QLightDM::Greeter::PromptType type);
    void authenticationComplete();
    void chooseUserMode();
    void chooseSessionMode();

private:
    BackgroundLabel* m_backgroundLabel;
    LogoWidget* m_logoWidget;
    SwitchFrame* m_switchFrame;
    UserWidget* m_userWidget;
    PassWdEdit* m_passWdEdit;
    SessionWidget *m_sessionWidget;
    QHBoxLayout* m_passWdEditLayout;
    QVBoxLayout* m_Layout;

    QLightDM::Greeter *m_greeter;
};
#endif // LoginManager

