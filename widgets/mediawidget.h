#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

#include "dbus/dbusmediaplayer2.h"

#include <QWidget>
#include <dimagebutton.h>
#include <dmpriscontrol.h>

DWIDGET_USE_NAMESPACE

class MediaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MediaWidget(QWidget *parent = 0);
    void initMediaPlayer();

private slots:
    void onVolumeChanged();
    void onMuteSwitchChanged();
    void changeVisible();

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();

private:
    DImageButton *m_volumeBtn;
    DMPRISControl *m_dmprisWidget;
    QLabel *m_volumeNums;
    double m_lastVolumeNums;
    DBusMediaPlayer2 *m_dbusInter;
};

#endif // MEDIAWIDGET_H
