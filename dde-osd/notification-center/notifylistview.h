#ifndef NOTIFYLISTVIEW_H
#define NOTIFYLISTVIEW_H

#include <QListView>

class NotifyListView : public QListView
{

public:
    NotifyListView(QWidget *parent = nullptr);

    void scrollTo(const QModelIndex &index, ScrollHint hint = EnsureVisible) override;
};

#endif // NOTIFYLISTVIEW_H
