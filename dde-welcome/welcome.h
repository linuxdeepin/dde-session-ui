#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

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
    void showEvent(QShowEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private:
    void dbus_show();
    void dbus_exit();

private slots:
    void overrideCursor();
    void onScreenRectChanged();

private:
    QTimer *m_sizeAdjustTimer;
};

#endif // WELCOME_H
