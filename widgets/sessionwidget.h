#ifndef SESSIONWIDGET_H
#define SESSIONWIDGET_H

#include <QWidget>

#include <QLightDM/SessionsModel>

class SessionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SessionWidget(QWidget *parent = 0);

    const QString currentSessionName() const;
    const QString currentSessionKey() const;

private:
    QLightDM::SessionsModel *m_sessionModel;
};

#endif // SESSIONWIDGET_H
