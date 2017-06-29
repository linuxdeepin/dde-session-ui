#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

#include <com_deepin_daemon_display.h>

using DisplayInter = com::deepin::daemon::Display;

class Welcome : public QWidget
{
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = 0);
    ~Welcome();

    void dbus_show();
    void dbus_exit();

private:
    void keyPressEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);

private:
    void onScreenRectChanged();

private:
    DisplayInter *m_displayInter;
    QTimer *m_sizeAdjustTimer;
};

#endif // WELCOME_H
