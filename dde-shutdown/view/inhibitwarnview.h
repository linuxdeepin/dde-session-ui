#ifndef INHIBITWARNVIEW_H
#define INHIBITWARNVIEW_H

#include "common.h"

#include <QWidget>

#include <dimagebutton.h>

DWIDGET_USE_NAMESPACE

class InhibitWarnView : public QWidget
{
    Q_OBJECT

public:
    explicit InhibitWarnView(QWidget *parent = 0);

    void setInhibitReason(const QString &reason);
    void setAcceptReason(const QString &reason);
    void setAction(const Actions action);
    void setAcceptVisible(const bool acceptable);

signals:
    void cancelled() const;
    void actionInvoked(const Actions action) const;

private:
    Actions m_action;

    QLabel *m_reasonLbl;
    QLabel *m_acceptLbl;
    DImageButton *m_acceptBtn;
    DImageButton *m_cancelBtn;
};

#endif // INHIBITWARNVIEW_H
