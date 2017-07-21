#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "fullscreenbackground.h"

class LoginWindow : public FullscreenBackground
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
};

#endif // LOGINWINDOW_H
