#include <QtCore/QObject>
#include <QtCore/QFile>
#include "leftcornerwidget.h"
#include <QDebug>
LogoWidget::LogoWidget(QWidget *parent)
    : QWidget(parent)
{
    m_logoLabel = new QLabel();
    m_logoLabel->setObjectName("Logo");
    m_logoLabel->setFixedHeight(150);

    m_logoVersionLabel = new QLabel("Alpha");
    m_logoVersionLabel->setObjectName("LogoVersion");
    m_logoVersionLabel->setFixedWidth(30);

    this->setObjectName("LogoWidget");
    m_logoLeftLayout = new QVBoxLayout;
    m_logoLeftLayout->setMargin(0);
    m_logoLeftLayout->setSpacing(0);
    m_logoLeftLayout->addStretch();
    m_logoLeftLayout->addWidget(m_logoLabel);

    m_logoRightLayout = new QVBoxLayout;
    m_logoRightLayout->setMargin(0);
    m_logoRightLayout->setSpacing(0);
    m_logoRightLayout->addWidget(m_logoVersionLabel);
    m_logoRightLayout->addStretch();

    m_logoLayout = new QHBoxLayout;
    m_logoLayout->setMargin(0);
    m_logoLayout->setSpacing(0);
    m_logoLayout->addLayout(m_logoLeftLayout);
    m_logoLayout->addLayout(m_logoRightLayout);
    setLayout(m_logoLayout);
}
LogoWidget::~LogoWidget()
{

}
TimeWidget::TimeWidget(QWidget *parent)
    : QWidget(parent)
{

    m_date = QDate::currentDate();
    m_time = QTime::currentTime();
    int yyy = m_date.year();
    int mmm = m_date.month();
    int ddd = m_date.day();
    int minute = m_time.hour();
    int second = m_time.minute();

    m_dateLabel = new QLabel;
    m_dateLabel->setFixedHeight(60);
    m_dateLabel->setText(QString("%1-%2-%3").arg(yyy).arg(mmm).arg(ddd));
    m_dateLabel->setStyleSheet("color: white; font-size: 15pt;text-align:center;");

    m_dayofweekLabel = new QLabel;
    m_dayofweekLabel->setFixedHeight(60);
    m_dayofweekLabel->setText(dayofweek(m_date.dayOfWeek()));
    m_dayofweekLabel->setStyleSheet("color: white; font-size: 15pt;text-align:center;");
    m_dateLayout = new QHBoxLayout;
    m_dateLayout->setMargin(0);
    m_dateLayout->setSpacing(5);
    m_dateLayout->addWidget(m_dateLabel);
    m_dateLayout->addWidget(m_dayofweekLabel);

    m_timeLabel = new QLabel;
    m_timeLabel->setFixedHeight(80);
    m_timeLabel->setText(QString("%1:%2").arg(minute).arg(second));
    m_timeLabel->setStyleSheet("color: white;font-size:65px;text-align:center;");


    m_totalTimeLayout = new QVBoxLayout;
    m_totalTimeLayout->setMargin(0);
    m_totalTimeLayout->setSpacing(0);
    m_totalTimeLayout->addStretch();
    m_totalTimeLayout->addWidget(m_timeLabel);
    m_totalTimeLayout->addLayout(m_dateLayout);
    m_totalTimeLayout->addStretch();
    setLayout(m_totalTimeLayout);
}
QString TimeWidget::dayofweek(int num) {
    switch(num) {
        case 1: { return "Monday";}
        case 2: { return "Tuesday";}
        case 3: { return "Wednesday";}
        case 4: { return "Thursday";}
        case 5: { return "Friday";}
        case 6: { return "Saturday";}
        default: { return "Sunday";}
     }
}
TimeWidget::~TimeWidget()
{
}
