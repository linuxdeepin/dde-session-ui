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

private:
    void dbus_show();
    void dbus_exit();

private slots:
    void clearCursor();
    void onScreenRectChanged();

private:
    QTimer *m_sizeAdjustTimer;
};

#endif // WELCOME_H
