/**
 * Copyright (C) 2016 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <QObject>
#include <QSqlDatabase>

class NotificationEntity;
class Persistence : public QObject
{
    Q_OBJECT
public:
    explicit Persistence(QObject *parent = 0);

    void addOne(NotificationEntity *entity);
    void addAll(QList<NotificationEntity*> entities);
    void removeOne(const QString &id);
    void removeAll();

    QList<NotificationEntity> getAll();

signals:
    void RecordAdded(NotificationEntity *entify);

private:
//    QList<NotificationEntity*> m_buffer;
    QSqlDatabase m_dbConnection;
};

#endif // PERSISTENCE_H
