#include "processinfotable.h"
#include "processinfomodel.h"

#include <QHeaderView>
#include <QDebug>

ProcessInfoTable::ProcessInfoTable(QWidget *parent)
    : QTableView(parent)
{
    horizontalHeader()->setVisible(false);
    verticalHeader()->setVisible(false);
    setFrameStyle(QFrame::NoFrame);
    setSelectionMode(NoSelection);
}

void ProcessInfoTable::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);

    // adjust section size
    QHeaderView *vHeader = verticalHeader();
    vHeader->setDefaultSectionSize(40);

    QHeaderView *hHeader = horizontalHeader();
    hHeader->setSectionResizeMode(COLUMN_ICON, QHeaderView::Fixed);
    hHeader->setSectionResizeMode(COLUMN_NAME, QHeaderView::Stretch);
    hHeader->setSectionResizeMode(COLUMN_MEM, QHeaderView::ResizeToContents);

    setColumnWidth(0, 24 + 10);
}

void ProcessInfoTable::mouseReleaseEvent(QMouseEvent *e)
{
    QTableView::mouseReleaseEvent(e);

    static_cast<ProcessInfoModel *>(model())->clearPressed();
}
