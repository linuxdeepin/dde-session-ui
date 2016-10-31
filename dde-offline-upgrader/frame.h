#ifndef FRAME_H
#define FRAME_H

#include <QWidget>

class QLabel;
class DeepinWave;
class DBusUpdateJob;
class DBusUpdateJobManager;
class Frame : public QWidget
{
    Q_OBJECT

public:
    Frame(QWidget *parent = 0);
    ~Frame();

protected:
    void resizeEvent(QResizeEvent *);

public slots:
    void showFullScreen();
    void distUpgrade();

private:
    DeepinWave * m_wave;
    QLabel * m_progressTip;
    QLabel * m_warningTip;
    DBusUpdateJobManager *m_dbusJobManagerInter;

private slots:
    void updateProgress(double progress);
    void tryReboot();
};

#endif // FRAME_H
