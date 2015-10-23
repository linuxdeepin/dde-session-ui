#include "lockframe.h"
#include "backgroundlabel.h"
#include "timewidget.h"
#include "userwidget.h"
#include "passwdedit.h"
#include "util_updateui.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDebug>

LockFrame::LockFrame()
{
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::FramelessWindowHint | Qt::SplashScreen);
    setFixedSize(qApp->desktop()->size());

    // background image
    BackgroundLabel *background = new BackgroundLabel(true, this);
    background->move(0, 0);
    TimeWidget *timeWidget = new TimeWidget(this);
    timeWidget->setFixedSize(400, 300);
//    timeWidget->setStyleSheet("background-color:red;");
    timeWidget->move(48, qApp->desktop()->height() - timeWidget->height() - 36); // left 48px and bottom 36px
    UserWidget *userWidget = new UserWidget(this);
    userWidget->move(0, (qApp->desktop()->height() - userWidget->height()) / 2 - 95);
    PassWdEdit *passwordEdit = new PassWdEdit("LockIcon", this);
    passwordEdit->setFocusPolicy(Qt::StrongFocus);
    passwordEdit->setFocus();

    QHBoxLayout *passwdLayout = new QHBoxLayout;
    passwdLayout->setMargin(0);
    passwdLayout->setSpacing(0);
    passwdLayout->addStretch();
    passwdLayout->addWidget(passwordEdit);
    passwdLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);
    mainLayout->addStretch();
    mainLayout->addLayout(passwdLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
    showFullScreen();
    activateWindow();
    updateStyle(":/theme/theme/lock.qss", this);
}

void LockFrame::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    default:;
    }
}

