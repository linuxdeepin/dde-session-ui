#ifndef DEEPINWAVE_H
#define DEEPINWAVE_H

#include <QWidget>

class QTimer;
class DeepinWave : public QWidget
{
    Q_OBJECT
public:
    explicit DeepinWave(QWidget *parent = 0);

    float progress() const;
    void setProgress(const float &progress);

protected:
    void paintEvent(QPaintEvent * event);

private:
    u_int m_count = 0;
    float m_progress = 0.5;

    QTimer * m_waveTimer;
};

#endif // DEEPINWAVE_H
