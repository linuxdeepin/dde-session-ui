#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class TimeWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimeWidget(QWidget *parent = 0);

private:
    void refreshTime();

private:
    QLabel *m_timeLabel;
    QLabel *m_dateLabel;

    QTimer *m_refreshTimer;
};

#endif // TIMEWIDGET_H
