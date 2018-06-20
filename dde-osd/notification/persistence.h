/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>

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

    QString getAll();
    QString getById(const QString &id);

    // the result starts with offset + 1
    // If rowcount is - 1, it is obtained from offset + 1 to the last.
    QString getFrom(int rowCount, const QString &offsetId);

signals:
    void RecordAdded(NotificationEntity *entify);

private:
    void attemptCreateTable();

private:
    QSqlDatabase m_dbConnection;
    QSqlQuery m_query;
};

#endif // PERSISTENCE_H
