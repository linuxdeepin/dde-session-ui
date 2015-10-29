#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QLabel>

#include <libdui/dimagebutton.h>

DUI_USE_NAMESPACE

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);

signals:
    void switchSession() const;

private:
    QLabel *m_songName;
    DImageButton *m_prevSong;
    DImageButton *m_pauseSong;
    DImageButton *m_nextSong;
    DImageButton *m_muteSong;
    DImageButton *m_sessions;
    DImageButton *m_shutdown;

    QWidget *m_songControlWidget;
};

#endif // CONTROLWIDGET_H
