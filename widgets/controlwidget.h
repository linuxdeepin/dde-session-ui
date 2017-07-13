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

public slots:
    void setMPRISEnable(const bool state);
    void setUserSwitchEnable(const bool visible);

private:
    void initUI();
    void initConnect();

private:
    QHBoxLayout *m_mainLayout;
    DImageButton *m_switchUserBtn;
    DImageButton *m_powerBtn;
    MediaWidget *m_mediaWidget;
};

#endif // CONTROLWIDGET_H
