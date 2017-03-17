#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>

#include "common.h"

class Container;
class ListView;
class Delegate;
class Model;
class AbstractOSDProvider;
class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = 0);

public slots:
    void ShowOSD(const QString &osd);

private:
    Container *m_container;
    ListView *m_listview;
    Delegate *m_delegate;
    Model *m_model;

    AbstractOSDProvider *m_currentProvider;
    AbstractOSDProvider *m_kbLayoutProvider;
    QList<AbstractOSDProvider*> m_providers;

    QTimer *m_timer;

private slots:
    void updateUI();
    void doneSetting();
};

#endif // MANAGER_H
