/*
 * Copyright (C) 2017 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     zccrs <zccrs@live.com>
 *
 * Maintainer: zccrs <zhangjide@deepin.com>
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
#ifndef PROPERTYGROUP_H
#define PROPERTYGROUP_H

#include <QObject>
#include <QPointer>
#include <QMap>

QT_BEGIN_NAMESPACE
class QSignalMapper;
QT_END_NAMESPACE
class PropertyGroup : public QObject
{
    Q_OBJECT
    // todo
    // Q_PROPERTY(bool enable ...)
public:
    explicit PropertyGroup(QObject *parent = nullptr);

    int addObject(QObject *obj);
    void removeObject(QObject *obj);
    // todo
    // removeObject(int index)
    // takeObject(int index);
    // QList<QObject*> objectList();
    void addProperty(const QByteArray &propertyName);
    void removeProperty(const QByteArray &propertyName);
    // todo
    // QByteArrayList propertyList() const;

private:
    void onObjectPropertyChanged(QObject *obj);

    QList<QObject*> m_objList;
    QByteArrayList m_propertyList;
    QMap<QByteArray, QSignalMapper*> m_signalMapperMap;
};

#endif // PROPERTYGROUP_H
