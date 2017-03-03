#ifndef KEYBOARDMONITOR_H
#define KEYBOARDMONITOR_H

#include <QThread>
#include <QX11Info>

class KeyboardMonitor : public QThread
{
    Q_OBJECT

public:
    static KeyboardMonitor *instance();

    bool isCapslockOn();
    bool isNumlockOn();
    bool setNumlockStatus(const bool &on);

signals:
    void capslockStatusChanged(bool on);
    void numlockStatusChanged(bool on);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    KeyboardMonitor();

    int listen(Display *display);
};

#endif // KEYBOARDMONITOR_H
