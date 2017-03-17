#ifndef MODEL_H
#define MODEL_H

#include <QAbstractListModel>

class AbstractOSDProvider;
class Model : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void setProvider(AbstractOSDProvider *provider);

private:
    AbstractOSDProvider *m_provider;
};

#endif // MODEL_H
