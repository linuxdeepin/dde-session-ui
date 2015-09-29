#ifndef LOGOWIDGET
#define LOGOWIDGET

#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QDate>
#include <QtCore/QTime>

class LogoWidget: public QWidget
{
    Q_OBJECT
public:
    LogoWidget(QWidget *parent=0);
    ~LogoWidget();
private:
    QLabel* m_logoLabel;
    QLabel* m_logoVersionLabel;
    QVBoxLayout* m_logoLeftLayout;
    QVBoxLayout* m_logoRightLayout;
    QHBoxLayout* m_logoLayout;
};
class TimeWidget: public QWidget
{
    Q_OBJECT
public:
    TimeWidget(QWidget *parent=0);
    ~TimeWidget();
public slots:
    QString dayofweek(int num);
private:
    QDate m_date;
    QTime m_time;
    QVBoxLayout* m_totalTimeLayout;
    QHBoxLayout* m_dateLayout;
    QLabel* m_dateLabel;
    QLabel* m_timeLabel;
    QLabel* m_dayofweekLabel;
};
#endif // LOGOWIDGET
