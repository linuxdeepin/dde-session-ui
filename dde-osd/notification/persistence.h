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

#include "constants.h"

#define ACTION_SEGMENT ("|")
#define HINT_SEGMENT ("|")
#define KEYVALUE_SEGMENT ("!!!")

class NotificationEntity;
class Persistence : public QObject
{
    Q_OBJECT
public:
    explicit Persistence(QObject *parent = nullptr);

    void addOne(EntityPtr entity);
    void addAll(QList<EntityPtr> entities);
    void removeOne(const QString &id);
    void removeApp(const QString &app_name);
    void removeAll();

    QList<EntityPtr> getAllNotify();
    QString getAll();
    QString getById(const QString &id);

    // the result starts with offset + 1
    // If rowcount is - 1, it is obtained from offset + 1 to the last.
    QString getFrom(int rowCount, const QString &offsetId);

    int getRecordCount();

signals:
    void RecordAdded(EntityPtr entity);

private:
    void attemptCreateTable();
    QString ConvertMapToString(const QVariantMap &map);
    QVariantMap ConvertStringToMap(const QString &text);

    bool IsAttributeValid(const QString &tableName, const QString &attributeName);
    bool AddAttributeToTable(const QString &tableName, const QString &attributeName);

private:
    QSqlDatabase m_dbConnection;
    QSqlQuery m_query;
};

#endif // PERSISTENCE_H
