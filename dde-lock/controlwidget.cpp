#include "controlwidget.h"

#include <QHBoxLayout>

ControlWidget::ControlWidget(QWidget *parent)
    : QWidget(parent)
{
    m_songName = new QLabel;
    m_songName->setStyleSheet("color:white;"
                              "background-color:red;"
                              "font-size:24px;"
                              "margin:0 0 15px 25px;");
    m_prevSong = new DImageButton;
    m_prevSong->setNormalPic(":/icons/icons/previous_normal.png");
    m_prevSong->setHoverPic(":/icons/icons/previous_hover.png");
    m_prevSong->setPressPic(":/icons/icons/previous_press.png");
    m_pauseSong = new DImageButton;
    m_pauseSong->setNormalPic(":/icons/icons/pause_normal.png");
    m_pauseSong->setHoverPic(":/icons/icons/pause_hover.png");
    m_pauseSong->setPressPic(":/icons/icons/pause_press.png");
    m_nextSong = new DImageButton;
    m_nextSong->setNormalPic(":/icons/icons/next_normal.png");
    m_nextSong->setHoverPic(":/icons/icons/next_hover.png");
    m_nextSong->setPressPic(":/icons/icons/next_press.png");
    m_muteSong = new DImageButton;
    m_muteSong->setNormalPic(":/icons/icons/volume_normal.png");
    m_muteSong->setHoverPic(":/icons/icons/volume_hover.png");
    m_muteSong->setPressPic(":/icons/icons/volume_press.png");
    m_shutdown = new DImageButton;
    m_shutdown->setNormalPic(":/icons/icons/shutdown_normal.png");
    m_shutdown->setHoverPic(":/icons/icons/shutdown_hover.png");
    m_shutdown->setPressPic(":/icons/icons/shutdown_press.png");

    // TODO: remove
    m_songName->setText("song name");

    QHBoxLayout *songCtrls = new QHBoxLayout;
    songCtrls->addWidget(m_prevSong);
    songCtrls->addWidget(m_pauseSong);
    songCtrls->addWidget(m_nextSong);
    songCtrls->addWidget(m_muteSong);
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
//    mainLayout->addSpacing(50);
    mainLayout->addWidget(m_shutdown);
    mainLayout->setAlignment(m_shutdown, Qt::AlignBottom);

    setLayout(mainLayout);
    setFixedSize(500, 150);
//    setStyleSheet("background-color:red;");
}

