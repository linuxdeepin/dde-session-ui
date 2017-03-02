//
// Created by hualet on 11/22/16.
//

#ifndef DDE_POLKIT_AGENT_ERRORTOOLTIP_H
#define DDE_POLKIT_AGENT_ERRORTOOLTIP_H

#include <darrowrectangle.h>

DWIDGET_USE_NAMESPACE

class ErrorTooltip : public DArrowRectangle
{
    Q_OBJECT
public:
    ErrorTooltip(QString errorMessage, QWidget *parent);
    ~ErrorTooltip(){}

    void setMessage(QString message);

private:
    QLabel *m_label;
};


#endif //DDE_POLKIT_AGENT_ERRORTOOLTIP_H
