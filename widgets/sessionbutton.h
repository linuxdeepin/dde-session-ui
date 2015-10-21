#ifndef SESSIONBUTTON_H
#define SESSIONBUTTON_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>

#include <libdui/dimagebutton.h>

class SessionButton : public QWidget
{
    Q_OBJECT
public:
    explicit SessionButton(QWidget *parent = 0);

    void setName(const QString &name);
    void show(const QPoint &position);

private:
    DUI::DImageButton *m_sessionIcon;
    QLabel *m_sessionName;

    QPropertyAnimation *m_showAnimation;
    QPropertyAnimation *m_hideAnimation;
};

#endif // SESSIONBUTTON_H
