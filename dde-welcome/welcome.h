#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

#include <com_deepin_daemon_display.h>

using DisplayInter = com::deepin::daemon::Display;

class WelcomeService;
class Welcome : public QWidget
{
    Q_OBJECT

    friend class WelcomeService;

public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();

private:
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    void dbus_show();
    void dbus_exit();

    void onScreenRectChanged();

private:
    DisplayInter *m_displayInter;
    QTimer *m_sizeAdjustTimer;
};

#endif // WELCOME_H
