#include "loginwindow.h"
#include "app/loginmanager.h"

LoginWindow::LoginWindow(QWidget *parent)
    : FullscreenBackground(parent),

      m_loginFrame(std::make_unique<LoginManager>(this))
{
    setContent(m_loginFrame.get());

    connect(m_loginFrame.get(), &LoginManager::requestBackground, this, static_cast<void (LoginWindow::*)(const QString &)>(&LoginWindow::setBackground));
}
