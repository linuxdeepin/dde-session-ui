#include "controlwidget.h"

#include <QHBoxLayout>
#include <QEvent>
#include <QWheelEvent>

ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
{
    m_songName = new QLabel;
    m_songName->setStyleSheet("color:white;"
//                              "background-color:red;"
                              "font-size:15px;"
                              "margin:0 0 10px 5px;");
    m_volumeNums = new QLabel;
    m_volumeNums->hide();
    m_volumeNums->setStyleSheet("color:white;"
//                                "background-color:red;"
                                "font-size:13px;");
    m_prevSong = new DImageButton;
    m_prevSong->setNormalPic(":/icons/icons/previous_normal.png");
    m_prevSong->setHoverPic(":/icons/icons/previous_hover.png");
    m_prevSong->setPressPic(":/icons/icons/previous_press.png");
    m_pauseSong = new DImageButton;
    m_nextSong = new DImageButton;
    m_nextSong->setNormalPic(":/icons/icons/next_normal.png");
    m_nextSong->setHoverPic(":/icons/icons/next_hover.png");
    m_nextSong->setPressPic(":/icons/icons/next_press.png");
    m_volume = new DImageButton;
    m_volume->installEventFilter(this);

    m_userswitch = new DImageButton;
    m_userswitch->setNormalPic(":/icons/icons/userswitch_normal.png");
    m_userswitch->setHoverPic(":/icons/icons/userswitch_hover.png");
    m_userswitch->setPressPic(":/icons/icons/userswitch_press.png");


    m_shutdown = new DImageButton;
    m_shutdown->setNormalPic(":/icons/icons/shutdown_normal.png");
    m_shutdown->setHoverPic(":/icons/icons/shutdown_hover.png");
    m_shutdown->setPressPic(":/icons/icons/shutdown_press.png");

    QHBoxLayout *volumeLayout = new QHBoxLayout;
    volumeLayout->addWidget(m_volumeNums);
    volumeLayout->setSpacing(0);
    volumeLayout->setMargin(0);

    QWidget *volumeWidget = new QWidget;
    volumeWidget->setFixedWidth(30);
    volumeWidget->setLayout(volumeLayout);

    QHBoxLayout *songCtrls = new QHBoxLayout;
    songCtrls->addWidget(m_prevSong);
    songCtrls->addWidget(m_pauseSong);
    songCtrls->addWidget(m_nextSong);
    songCtrls->addWidget(m_volume);
    songCtrls->addWidget(volumeWidget);
    songCtrls->setSpacing(0);
    songCtrls->setMargin(0);

    QVBoxLayout *songLayout = new QVBoxLayout;
    songLayout->addWidget(m_songName);
    songLayout->addStretch();
    songLayout->addLayout(songCtrls);
    songLayout->setSpacing(0);
    songLayout->setMargin(0);

    m_songControlWidget = new QWidget;
    m_songControlWidget->setLayout(songLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(m_songControlWidget);
    mainLayout->setAlignment(m_songControlWidget, Qt::AlignBottom);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_userswitch);
    mainLayout->setAlignment(m_userswitch, Qt::AlignBottom);
    mainLayout->addWidget(m_shutdown);
    mainLayout->setAlignment(m_shutdown, Qt::AlignBottom);

    setLayout(mainLayout);
    setFixedSize(500, 150);
    //    setStyleSheet("background-color:red;");

    connect(m_shutdown, &DImageButton::clicked, this, &ControlWidget::shutdown);
    connect(m_userswitch, &DImageButton::clicked, this, &ControlWidget::switchToGreeter);
}

void ControlWidget::bindDBusService(DBusMediaPlayer2 *dbusInter)
{
    m_dbusInter = dbusInter;

    connect(m_prevSong, &DImageButton::clicked, m_dbusInter, &DBusMediaPlayer2::Previous);
    connect(m_nextSong, &DImageButton::clicked, m_dbusInter, &DBusMediaPlayer2::Next);
    connect(m_pauseSong, &DImageButton::clicked, m_dbusInter, &DBusMediaPlayer2::PlayPause);
    connect(m_dbusInter, &DBusMediaPlayer2::PlaybackStatusChanged, this, &ControlWidget::changePauseBtnPic);
    connect(m_dbusInter, &DBusMediaPlayer2::VolumeChanged, this, &ControlWidget::changeVolumeBtnPic);
    connect(m_dbusInter, &DBusMediaPlayer2::VolumeChanged, [this] {
        m_volumeNums->setText(QString::number(m_dbusInter->volume() * 100));
    });
    connect(m_dbusInter, &DBusMediaPlayer2::MetadataChanged, [this] {
        m_songName->setText(m_dbusInter->metadata().value("xesam:title").toString());
    });
    connect(m_volume, &DImageButton::clicked, [this] {
        const double currentVolume = m_dbusInter->volume();

        if (currentVolume) {
            m_lastVolumeNums = currentVolume;
            m_dbusInter->setVolume(0);
        } else {
            m_dbusInter->setVolume(m_lastVolumeNums);
        }
    });

    m_dbusInter->VolumeChanged();
    m_dbusInter->MetadataChanged();
    m_dbusInter->PlaybackStatusChanged();
    m_dbusInter->VolumeChanged();
}

void ControlWidget::hideMusicControlWidget()
{
    m_songControlWidget->hide();
}

void ControlWidget::showMusicControlWidget()
{
    m_songControlWidget->show();
}

bool ControlWidget::eventFilter(QObject *o, QEvent *e)
{
    if (o == m_volume) {
        switch (e->type()) {
        case QEvent::Wheel:     volumeWheelControl(reinterpret_cast<QWheelEvent *>(e));         break;
        case QEvent::Enter:     m_volumeNums->show();       break;
        case QEvent::Leave:     m_volumeNums->hide();       break;
        default:;
        }
    }

    return false;
}

void ControlWidget::volumeWheelControl(const QWheelEvent *e)
{
    const int delta = e->delta() / 60;

    m_lastVolumeNums += double(delta) / 100;
    if (m_lastVolumeNums < 0)
        m_lastVolumeNums = 0;
    if (m_lastVolumeNums > 1.0)
        m_lastVolumeNums = 1.0;

    m_dbusInter->setVolume(m_lastVolumeNums);
}

void ControlWidget::changePauseBtnPic()
{
    if (m_dbusInter->playbackStatus() == "Playing") {
        m_pauseSong->setNormalPic(":/icons/icons/pause_normal.png");
        m_pauseSong->setHoverPic(":/icons/icons/pause_hover.png");
        m_pauseSong->setPressPic(":/icons/icons/pause_press.png");
    } else {
        m_pauseSong->setNormalPic(":/icons/icons/start_normal.png");
        m_pauseSong->setHoverPic(":/icons/icons/start_hover.png");
        m_pauseSong->setPressPic(":/icons/icons/start_press.png");
    }
}

void ControlWidget::changeVolumeBtnPic()
{
    if (m_dbusInter->volume()) {
        m_volume->setNormalPic(":/icons/icons/volume_normal.png");
        m_volume->setHoverPic(":/icons/icons/volume_hover.png");
        m_volume->setPressPic(":/icons/icons/volume_press.png");
    } else {
        m_volume->setNormalPic(":/icons/icons/mute_normal.png");
        m_volume->setHoverPic(":/icons/icons/mute_hover.png");
        m_volume->setPressPic(":/icons/icons/mute_press.png");
    }
}

void ControlWidget::shutdown()
{
//    QStringList args;
//    args << "-H" << "lock";
//    args << "-H" << "switchuser";
//    args << "-H" << "logout";

    QProcess *process = new QProcess;
//    process->setArguments(args);

    connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), process, &QProcess::deleteLater);

    process->start("dde-shutdown -H lock -H switchuser -H logout");
}

void ControlWidget::switchToGreeter() {
    QProcess *process = new QProcess;
    connect(process, static_cast<void (QProcess::*)(int)>(&QProcess::finished), process, &QProcess::deleteLater);
    process->start("dde-switchtogreeter");
    qApp->quit();
}
