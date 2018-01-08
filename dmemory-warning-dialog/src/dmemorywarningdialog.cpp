#include "dmemorywarningdialog.h"
#include "processinfotable.h"
#include "processinfomodel.h"
#include "processinfodelegate.h"
#include "buttondelegate.h"

#include <QKeyEvent>
#include <QApplication>
#include <QScreen>

DMemoryWarningDialog::DMemoryWarningDialog(QWidget *parent)
    : QMainWindow(parent)
{
    ProcessInfoTable *table = new ProcessInfoTable;
    table->setModel(new ProcessInfoModel);
    table->setItemDelegate(new ProcessInfoDelegate);
    table->setItemDelegateForColumn(3, new ButtonDelegate);

    setCentralWidget(table);
    setFixedSize(400, 600);
    move(qApp->primaryScreen()->geometry().center() - rect().center());
}

void DMemoryWarningDialog::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef QT_DEBUG
    case Qt::Key_Escape:    qApp->quit();   break;
#endif
    default:;
    }
}
