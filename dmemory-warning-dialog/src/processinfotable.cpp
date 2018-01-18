#include "processinfotable.h"
#include "processinfomodel.h"

#include <QHeaderView>
#include <QDebug>
#include <QPainter>

ProcessInfoView::ProcessInfoView(QWidget *parent)
    : QTreeView(parent)
{
    header()->setVisible(false);
//    horizontalHeader()->setVisible(false);
//    verticalHeader()->setVisible(false);
//    setRootIsDecorated(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    setShowGrid(false);
    setFrameStyle(QFrame::NoFrame);
    setSelectionMode(NoSelection);
    setStyleSheet("QTreeView {"
                  "border: 1px solid #ccc;"
                  "border-radius: 5px;"
                  "}"
                  ""
                  "QTreeView::branch:!has-children {"
                  "border: none;"
                  "}"
                  ""
                  "QTreeView::branch:has-children {"
//                  "border: 1px solid red;"
                  "}"
                  "");
}

void ProcessInfoView::setModel(QAbstractItemModel *model)
{
    QTreeView::setModel(model);

    // adjust section size
//    QHeaderView *vHeader = verticalHeader();
//    vHeader->setDefaultSectionSize(40);

//    QHeaderView *hHeader = horizontalHeader();
//    hHeader->setSectionResizeMode(COLUMN_ICON, QHeaderView::Fixed);
//    hHeader->setSectionResizeMode(COLUMN_NAME, QHeaderView::Stretch);
//    hHeader->setSectionResizeMode(COLUMN_MEM, QHeaderView::ResizeToContents);

//    QHeaderView *header = this->header();
//    header->setSectionResizeMode(COLUMN_ICON, QHeaderView::Fixed);
//    header->setSectionResizeMode(COLUMN_NAME, QHeaderView::Stretch);
//    header->setSectionResizeMode(COLUMN_MEM, QHeaderView::ResizeToContents);

    header()->setSectionResizeMode(COLUMN_ICON, QHeaderView::Fixed);
    header()->setSectionResizeMode(COLUMN_NAME, QHeaderView::Stretch);
    header()->setSectionResizeMode(COLUMN_MEM, QHeaderView::ResizeToContents);
//    header()->setSectionResizeMode(COLUMN_FREE_BTN, QHeaderView::Fixed);
    setColumnWidth(COLUMN_ICON, 24 + 10 + 30);
    setColumnWidth(COLUMN_FREE_BTN, 10);
//    setColumnWidth(3, 10);
}

void ProcessInfoView::mouseReleaseEvent(QMouseEvent *e)
{
    QTreeView::mouseReleaseEvent(e);

    static_cast<ProcessInfoModel *>(model())->clearPressed();
}

void ProcessInfoView::drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const
{
    if (index.parent().isValid())
    {
        const QRect r = options.rect.marginsRemoved(QMargins(40, 0, 0, 0));
        const QPixmap &iconPix = index.data(ProcessInfoModel::IconRole).value<QPixmap>();

        painter->drawPixmap(r.topLeft(), iconPix);
        painter->drawText(r.marginsRemoved(QMargins(30, 0, 0, 0)),
                          Qt::AlignVCenter | Qt::AlignLeft,
                          index.sibling(index.row(), 1).data(ProcessInfoModel::DisplayRole).toString());

        return;
    }

    QTreeView::drawRow(painter, options, index);
}
