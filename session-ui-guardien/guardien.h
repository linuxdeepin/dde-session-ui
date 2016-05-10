#ifndef GUARDIEN_H
#define GUARDIEN_H

#include <QObject>
#include <QProcess>

class Guardien : public QObject
{
    Q_OBJECT
public:
    explicit Guardien(QObject *parent = 0);
    void start();

private:
    QProcess *m_shutdown;
    QProcess *m_lock;

    void restart();

private slots:
    void handleFinished(int);
};

#endif // GUARDIEN_H
