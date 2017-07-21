#include "loginwindow.h"
#include "app/loginmanager.h"

LoginWindow::LoginWindow(QWidget *parent)
    : FullscreenBackground(parent)
{
    setContent(new LoginManager(this));
}
