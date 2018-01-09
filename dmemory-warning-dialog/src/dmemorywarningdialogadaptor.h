#ifndef DMEMORYWARNINGDIALOGADAPTOR_H
#define DMEMORYWARNINGDIALOGADAPTOR_H

#include "dmemorywarningdialog.h"

#include <QDBusAbstractAdaptor>

class DMemoryWarningDialogAdaptor : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.MemoryWarningDialog")

public:
    explicit DMemoryWarningDialogAdaptor(DMemoryWarningDialog *dialog);
    ~DMemoryWarningDialogAdaptor();

public Q_SLOTS:
    void Show();
    void Hide();

private:
    DMemoryWarningDialog *dialog;
};

#endif // DMEMORYWARNINGDIALOGADAPTOR_H
