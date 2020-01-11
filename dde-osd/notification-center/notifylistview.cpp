#include "notifylistview.h"

NotifyListView::NotifyListView(QWidget *parent)
    :QListView(parent)
{

}

void NotifyListView::scrollTo(const QModelIndex &index, ScrollHint hint)
{
    Q_UNUSED(index)
    Q_UNUSED(hint)

    return;
}
