#ifndef MAINWINDOWWITHCLICK_H
#define MAINWINDOWWITHCLICK_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QKeyEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(int width, int height, QWidget *parent = 0);
    ~MainWindow();

    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

private:
    QStringList m_ButtonNames;
    QStringList m_ActionStrs;
    QStringList m_ActionStrs2;

    // all the actions' dbus-about string
    const QString CONTROL_CENTER_FROM_LEFT_STR = "dbus-send --print-reply --dest=com.deepin.dde.ControlCenter /com/deepin/dde/ControlCenter com.deepin.dde.ControlCenter.ToggleInLeft";
    const QString CONTROL_CENTER_FROM_RIGHT_STR = "dbus-send --print-reply --dest=com.deepin.dde.ControlCenter /com/deepin/dde/ControlCenter com.deepin.dde.ControlCenter.Toggle";
    const QString ALL_WINDOWS_STR = "/usr/bin/xdotool key Super+w";
    const QString LAUNCHER_STR = "/usr/bin/dde-launcher";
    const QString SHOW_DESKTOP_STR = "/usr/lib/deepin-daemon/desktop-toggle";
    const QString NONE_STR = "";
};

#endif // MAINWINDOWWITHCLICK_H
