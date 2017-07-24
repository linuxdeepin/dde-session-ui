#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "fullscreenbackground.h"
#include "app/loginmanager.h"

#include <memory>

class LoginWindow : public FullscreenBackground
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);

private:
    std::unique_ptr<LoginManager> m_loginFrame;
};

#endif // LOGINWINDOW_H
