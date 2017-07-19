#ifndef FULLSCREENBACKGROUND_H
#define FULLSCREENBACKGROUND_H

#include <QWidget>
#include <QPointer>
#include <QTimer>

class FullscreenBackground : public QWidget
{
    Q_OBJECT

public:
    explicit FullscreenBackground(QWidget *parent = 0);

protected:
    void setContent(QWidget * const w);

private slots:
    void adjustGeometry();

private:
    void showEvent(QShowEvent *e);

private:
    QPointer<QWidget> m_content;

    QTimer *m_adjustTimer;
};

#endif // FULLSCREENBACKGROUND_H
