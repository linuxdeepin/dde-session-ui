#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <dimagebutton.h>
#include <mediawidget.h>

DWIDGET_USE_NAMESPACE

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);

signals:
    void requestSwitchUser();
    void requestShutdown();
    void requestSwitchSession();

public slots:
    void setMPRISEnable(const bool state);
    void setUserSwitchEnable(const bool visible);
    void setSessionSwitchEnable(const bool visible);
    void chooseToSession(const QString &session);

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();
    void showTips();
    void hideTips();

private:
    QHBoxLayout *m_mainLayout = nullptr;
    DImageButton *m_switchUserBtn = nullptr;
    DImageButton *m_powerBtn = nullptr;
    MediaWidget *m_mediaWidget = nullptr;
    DImageButton *m_sessionBtn = nullptr;
    QLabel *m_sessionTip = nullptr;
    QWidget *m_tipWidget = nullptr;
#ifndef SHENWEI_PLATFORM
    QPropertyAnimation *m_tipsAni = nullptr;
#endif
};

#endif // CONTROLWIDGET_H
