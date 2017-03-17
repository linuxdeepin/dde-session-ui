#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QListView>

class ListView : public QListView
{
    Q_OBJECT
public:
    explicit ListView(QWidget *parent = 0);
};

#endif // LISTVIEW_H
