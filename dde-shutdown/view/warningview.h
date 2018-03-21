#ifndef WARNINGVIEW_H
#define WARNINGVIEW_H

#include <QFrame>

class WarningView : public QFrame
{
public:
    explicit WarningView(QWidget *parent = nullptr) : QFrame(parent) {}
    virtual void toggleButtonState() = 0;
    virtual void buttonClickHandle() = 0;
    virtual void setAcceptReason(const QString &reason) = 0;
};

#endif // WARNINGVIEW_H
