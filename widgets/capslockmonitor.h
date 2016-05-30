#ifndef CAPSLOCKMONITOR_H
#define CAPSLOCKMONITOR_H

#include <QThread>
#include <QX11Info>

class CapslockMonitor : public QThread
{
    Q_OBJECT

public:
    CapslockMonitor(QObject * parent = 0);

    bool isCapslockOn();

signals:
    void capslockStatusChanged(bool on);

protected:
    void run() Q_DECL_OVERRIDE;

private:
    int listen(Display *display);
};

#endif // CAPSLOCKMONITOR_H
