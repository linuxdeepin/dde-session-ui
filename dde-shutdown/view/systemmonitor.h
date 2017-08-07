#ifndef SYSTEMMONITOR_H
#define SYSTEMMONITOR_H

#include <QWidget>
#include <QLabel>

class SystemMonitor : public QWidget
{
    Q_OBJECT
public:
    explicit SystemMonitor(QWidget *parent = 0);

    enum State{
        Enter,
        Leave,
        Press,
        Release
    };

signals:
    void clicked();

public slots:
    void setState(State state);

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    QWidget *m_icon;
    QLabel *m_text;
    State m_state;
};

#endif // SYSTEMMONITOR_H
