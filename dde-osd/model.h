#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>

class Model : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void setData(const QList<QPair<QString, QString> > &data);

private:
    QList<QPair<QString, QString>> m_data;
};

#endif // MODEL_H
