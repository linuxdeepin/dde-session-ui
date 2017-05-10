#ifndef WARNINGDIALOG_H
#define WARNINGDIALOG_H

#include <ddialog.h>
#include <QWidget>

DWIDGET_USE_NAMESPACE

class WarningDialog : public DDialog
{
    Q_OBJECT

public:
    WarningDialog(QWidget *parent = 0);
    ~WarningDialog();
};

#endif // WARNINGDIALOG_H
