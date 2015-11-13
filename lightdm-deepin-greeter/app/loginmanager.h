#ifndef LoginManager_H
#define LoginManager_H

#include <QApplication>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>

#include <QLightDM/Greeter>
#include <QLightDM/SessionsModel>

#include "backgroundlabel.h"
#include "util_updateui.h"
#include "userwidget.h"
#include "passwdedit.h"
#include "view/logowidget.h"
#include "view/switchframe.h"
#include "sessionwidget.h"
#include "kblayoutwidget.h"
#include "shutdownwidget.h"
#include "xkbparser.h"
#include "dbus/dbusdisplaymanager.h"

class BackgroundLabel;
class LoginManager: public QFrame {
    Q_OBJECT
public:
    LoginManager(QWidget* parent=0);
    ~LoginManager();

protected:
    void keyPressEvent(QKeyEvent* e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* e) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *) Q_DECL_OVERRIDE;
signals:
    void leftKeyPressed();
    void rightKeyPressed();
private slots:
    void initUI();
    void initConnect();
    void login();
    void prompt(QString text, QLightDM::Greeter::PromptType type);
    void authenticationComplete();
    void chooseUserMode();
    void chooseSessionMode();
    void updateWidgetsPosition();
    void showShutdownFrame();
    void keyboardLayoutUI();
    void keybdLayoutWidget();
    void setCurrentKeyboardLayout(QString keyboard_value);

private:
    BackgroundLabel* m_backgroundLabel;
    LogoWidget* m_logoWidget;
    SwitchFrame* m_switchFrame;
    UserWidget* m_userWidget;
    PassWdEdit* m_passWdEdit;
    SessionWidget *m_sessionWidget;
    QHBoxLayout* m_passWdEditLayout;
    QVBoxLayout* m_Layout;

    ShutdownWidget* m_requireShutdownWidget;
    KbLayoutWidget* m_keybdLayoutWidget;
    XkbParser* xkbParse;

    QLightDM::Greeter *m_greeter;
    DBusDisplayManager *m_displayInter;
};
#endif // LoginManager

