#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDBusAbstractAdaptor>

class QLabel;
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
    QLabel * m_image;

    void setupSize();
    void setupImagePosition();
};

class LowPowerAdaptor : public QDBusAbstractAdaptor {
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.LowPower")

public:
    LowPowerAdaptor(Window * parent);
    ~LowPowerAdaptor();

public slots:
    void Raise();
    void Quit();
};

#endif // WINDOW_H
