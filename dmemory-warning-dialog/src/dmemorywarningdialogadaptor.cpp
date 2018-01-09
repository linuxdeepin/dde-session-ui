#include "dmemorywarningdialogadaptor.h"

DMemoryWarningDialogAdaptor::DMemoryWarningDialogAdaptor(DMemoryWarningDialog *dialog)
    : QDBusAbstractAdaptor(dialog)

    , dialog(dialog)
{

}

DMemoryWarningDialogAdaptor::~DMemoryWarningDialogAdaptor()
{

}

void DMemoryWarningDialogAdaptor::Show()
{
    dialog->show();
}

void DMemoryWarningDialogAdaptor::Hide()
{
    dialog->hide();
}
