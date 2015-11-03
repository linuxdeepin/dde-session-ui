#ifndef SWITCHFRAME
#define SWITCHFRAME

#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QDebug>

#include <libdui/dimagebutton.h>

DUI_USE_NAMESPACE

class SwitchFrame:public QFrame
{
    Q_OBJECT
public:
    SwitchFrame(QWidget* parent=0);
    ~SwitchFrame();

signals:
    void triggerSwitchSession();
    void triggerSwitchUser();
    void triggerPower();

public slots:
    void chooseToSession(const QString &tips);

protected:
    bool eventFilter(QObject *o, QEvent *e);

private:
    void initUI();
    void initConnect();

private slots:
    void hideTips();
    void showTips();

private:
    QLabel *m_tipsLabel;
    QWidget *m_tipsWidget;
    DImageButton* m_switchSessionButton;
    DImageButton* m_switchUserButton;
    DImageButton* m_powerButton;
    QHBoxLayout* m_Layout;

    QPropertyAnimation *m_tipsAni;
};
#endif // SWITCHFRAME

