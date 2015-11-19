#ifndef SESSIONWIDGET_H
#define SESSIONWIDGET_H

#include <QFrame>
#include <QList>

#include <QLightDM/SessionsModel>

#include "rounditembutton.h"

class SessionWidget : public QFrame
{
    Q_OBJECT
public:
    explicit SessionWidget(QWidget *parent = 0);
    ~SessionWidget();

    const QString currentSessionName() const;
    const QString currentSessionKey() const;

    void show();

public slots:
    void switchToUser(const QString &userName);
    void saveUserLastSession(const QString &userName);

signals:
    void sessionChanged(const QString &sessionName);

private slots:
    void switchSession(const QString &sessionName);
    int getSessionIndex(const QString &sessionName);

private:
    int m_currentSessionIndex;
    QString m_lastSelectedUser = QString();
    QList<RoundItemButton *> *m_sessionBtns;
    QLightDM::SessionsModel *m_sessionModel;
};

#endif // SESSIONWIDGET_H
