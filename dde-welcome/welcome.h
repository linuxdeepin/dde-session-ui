#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>

#include <dpicturesequenceview.h>

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

    Dtk::Widget::DPictureSequenceView *m_loadingSpinner;
};

#endif // WELCOME_H
