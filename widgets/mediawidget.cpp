#include "mediawidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWheelEvent>

MediaWidget::MediaWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
    initConnect();
}

void MediaWidget::initUI()
{
    m_dmprisWidget = new DMPRISControl;
    m_dmprisWidget->setFixedWidth(200);

    m_volumeBtn = new DImageButton;
    m_volumeBtn->setNormalPic(":/img/mpris/volume_normal.png");
    m_volumeBtn->setHoverPic(":/img/mpris/volume_hover.png");
    m_volumeBtn->setPressPic(":/img/mpris/volume_press.png");
    m_volumeBtn->installEventFilter(this);

    m_volumeNums = new QLabel;
    m_volumeNums->hide();

    QVBoxLayout *volumeLayout = new QVBoxLayout;
    volumeLayout->setMargin(0);
    volumeLayout->setSpacing(0);
    volumeLayout->addStretch();
    volumeLayout->addWidget(m_volumeNums, 0 , Qt::AlignHCenter);
    volumeLayout->addSpacing(15);
    volumeLayout->addWidget(m_volumeBtn);

    QHBoxLayout *mainlayout = new QHBoxLayout;
    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);

    mainlayout->addStretch();
    mainlayout->addWidget(m_dmprisWidget, 0, Qt::AlignBottom);
    mainlayout->addSpacing(26);
    mainlayout->addLayout(volumeLayout);

    setLayout(mainlayout);
}

void MediaWidget::initConnect()
{
    connect(m_volumeBtn, &DImageButton::clicked, this, &MediaWidget::onMuteSwitchChanged);
    connect(m_dmprisWidget, &DMPRISControl::mprisAcquired, this, &MediaWidget::changeVisible);
    connect(m_dmprisWidget, &DMPRISControl::mprisLosted, this, &MediaWidget::changeVisible);

    changeVisible();
}

void MediaWidget::initMediaPlayer()
{
    QDBusInterface dbusInter("org.freedesktop.DBus", "/", "org.freedesktop.DBus", QDBusConnection::sessionBus(), this);

    QDBusReply<QStringList> response = dbusInter.call("ListNames");
    const QStringList &serviceList = response.value();
    QString service = QString();
    for (const QString &serv : serviceList) {
        if (!serv.startsWith("org.mpris.MediaPlayer2.")) {
            continue;
        }
        service = serv;
        break;
    }
    if (service.isEmpty())
        return;

    qDebug() << "got service: " << service;

    m_dbusInter = new DBusMediaPlayer2(service, "/org/mpris/MediaPlayer2", QDBusConnection::sessionBus(), this);

    connect(m_dbusInter, &DBusMediaPlayer2::VolumeChanged, this, &MediaWidget::onVolumeChanged);
    connect(m_dbusInter, &DBusMediaPlayer2::VolumeChanged, [this] {
        m_volumeNums->setText(QString::number(m_dbusInter->volume() * 100));
    });

    m_lastVolumeNums = m_dbusInter->volume();

    m_dbusInter->VolumeChanged();
    m_dbusInter->MetadataChanged();
    m_dbusInter->PlaybackStatusChanged();
    m_dbusInter->VolumeChanged();
}

void MediaWidget::onVolumeChanged()
{
    if (m_dbusInter->volume()) {
        const double volume = m_dbusInter->volume();
        if (volume < 0.3) {
            m_volumeBtn->setNormalPic(":/img/mpris/volume3_normal.png");
            m_volumeBtn->setHoverPic(":/img/mpris/volume3_hover.png");
            m_volumeBtn->setPressPic(":/img/mpris/volume3_press.png");
        } else if (volume < 0.6) {
            m_volumeBtn->setNormalPic(":/img/mpris/volume2_normal.png");
            m_volumeBtn->setHoverPic(":/img/mpris/volume2_hover.png");
            m_volumeBtn->setPressPic(":/img/mpris/volume2_press.png");
        } else {
            m_volumeBtn->setNormalPic(":/img/mpris/volume_normal.png");
            m_volumeBtn->setHoverPic(":/img/mpris/volume_hover.png");
            m_volumeBtn->setPressPic(":/img/mpris/volume_press.png");
        }
    } else {
        m_volumeBtn->setNormalPic(":/img/mpris/mute_normal.png");
        m_volumeBtn->setHoverPic(":/img/mpris/mute_hover.png");
        m_volumeBtn->setPressPic(":/img/mpris/mute_press.png");
    }
}

void MediaWidget::onMuteSwitchChanged()
{
    const double currentVolume = m_dbusInter->volume();

    if (currentVolume) {
        m_lastVolumeNums = currentVolume;
        m_dbusInter->setVolume(0);
    } else {
        m_dbusInter->setVolume(m_lastVolumeNums);
    }
}

bool MediaWidget::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)

    switch (event->type()) {
    case QEvent::Wheel:
    {
        QWheelEvent *e  = static_cast<QWheelEvent *>(event);

        if (!e)
            break;

        const int delta = e->delta() / 60;

        m_lastVolumeNums += double(delta) / 100;

        if (m_lastVolumeNums < 0)
            m_lastVolumeNums = 0;
        if (m_lastVolumeNums > 1.0)
            m_lastVolumeNums = 1.0;


        m_dbusInter->setVolume(m_lastVolumeNums);
        break;
    }
    case QEvent::Enter:     m_volumeNums->show();       break;
    case QEvent::Leave:     m_volumeNums->hide();       break;
    default:;
    }

    return false;
}

void MediaWidget::changeVisible()
{
    const bool isWorking = m_dmprisWidget->isWorking();
    m_dmprisWidget->setVisible(isWorking);
    m_volumeBtn->setVisible(isWorking);
}
