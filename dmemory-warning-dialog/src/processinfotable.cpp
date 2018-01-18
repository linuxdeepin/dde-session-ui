#include "processinfotable.h"
#include "processinfomodel.h"

#include <QHeaderView>
#include <QDebug>
#include <QPainter>

ProcessInfoView::ProcessInfoView(QWidget *parent)
    : QTreeView(parent)
{
    header()->setVisible(false);
    setRootIsDecorated(false);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
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

    QHeaderView *header = this->header();
    header->setSectionsMovable(false);
    header->setStretchLastSection(false);
    header->setSectionResizeMode(COLUMN_ICON, QHeaderView::Fixed);
    header->setSectionResizeMode(COLUMN_NAME, QHeaderView::Stretch);
//    header->setSectionResizeMode(COLUMN_MEM, QHeaderView::ResizeToContents);

    header->resizeSection(COLUMN_ICON, 24 + 10 + 20);
    header->resizeSection(COLUMN_MEM, 50);
    header->resizeSection(COLUMN_FREE_BTN, 80);

    connect(model, &QAbstractItemModel::layoutChanged, this, [=] { expandAll(); });
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

        painter->fillRect(r, QColor(0, 0, 0, 255 * .035));
        painter->drawPixmap(r.topLeft(), iconPix);
        painter->drawText(r.marginsRemoved(QMargins(30, 0, 0, 0)),
                          Qt::AlignVCenter | Qt::AlignLeft,
                          index.sibling(index.row(), 1).data(ProcessInfoModel::DisplayRole).toString());

        return;
    }

    QTreeView::drawRow(painter, options, index);
}
