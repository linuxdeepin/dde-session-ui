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
    qDebug() << Q_FUNC_INFO << launchInfo;

    dialog->show();
    dialog->updateAppInfo(launchInfo);
}

void DMemoryWarningDialogAdaptor::Hide()
{
    qDebug() << Q_FUNC_INFO;

    dialog->hide();
}
