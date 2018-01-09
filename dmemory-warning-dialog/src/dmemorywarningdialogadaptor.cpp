#include "dmemorywarningdialogadaptor.h"

DMemoryWarningDialogAdaptor::DMemoryWarningDialogAdaptor(DMemoryWarningDialog *dialog)
    : QDBusAbstractAdaptor(dialog)

    , dialog(dialog)
{

}

DMemoryWarningDialogAdaptor::~DMemoryWarningDialogAdaptor()
{

}

void DMemoryWarningDialogAdaptor::Show(const QString &launchInfo)
{
    dialog->show();
    dialog->updateAppInfo(launchInfo);
}

void DMemoryWarningDialogAdaptor::Hide()
{
    dialog->hide();
}
