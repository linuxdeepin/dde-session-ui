#define private public
#include "dmemorywarningdialog.h"
#include "dmemorywarningdialogadaptor.h"
#undef private

#include <QApplication>
#include <QMimeData>
#include <QDBusConnection>
#include <QTest>

#include <gtest/gtest.h>

class UT_DMemoryWarningDialog : public testing::Test
{

};

TEST_F(UT_DMemoryWarningDialog, coverageTest)
{
    DMemoryWarningDialog dialog;
    DMemoryWarningDialogAdaptor dbusAdaptor(&dialog);
    QDBusConnection connection = QDBusConnection::sessionBus();
    if (connection.registerService("com.deepin.dde.MemoryWarningDialog") ||
            connection.registerObject("/com/deepin/dde/MemoryWarningDialog", &dialog)) {
        dialog.show();
        for (auto type = DMemoryWarningDialog::LaunchApp;
             type <= DMemoryWarningDialog::ExecuteCommand;
             type = (DMemoryWarningDialog::TipsType)(type +1)) {
            dialog.onNeedMemChanged(true);
            dialog.onNeedMemChanged(false);
        }

        QTest::keyPress(&dialog, Qt::Key_0);
        QTest::keyPress(&dialog, Qt::Key_Escape);
        dialog.show();
        dialog.hide();
    }
}
