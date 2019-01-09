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
#include "propertygroup.h"

#include <QMetaObject>
#include <QMetaProperty>
#include <QSignalMapper>
#include <QDebug>

PropertyGroup::PropertyGroup(QObject *parent)
    : QObject(parent)
{

}

int PropertyGroup::addObject(QObject *obj)
{
    m_objList.append(obj);
    int objIndex = m_objList.count() - 1;

    connect(obj, &QObject::destroyed, this, &PropertyGroup::removeObject);

    if (m_propertyList.isEmpty())
        return objIndex;

    const QMetaObject *mo = obj->metaObject();

    for (int i = 0; i < mo->propertyCount(); ++i) {
        auto p = mo->property(i);

        if (QSignalMapper *mapper = m_signalMapperMap.value(QByteArray(p.name()))) {
            Q_ASSERT(p.isReadable() && p.isWritable() && p.hasNotifySignal());

            // 如果目前已存在对象的这个属性为真，则将新加入对象的此属性设置为假
            for (const QObject *oo : m_objList) {
                if (oo != obj && oo->property(p.name()).toBool()) {
                    p.write(obj, QVariant());
                    break;
                }
            }

            static int mapper_mapping_index = mapper->metaObject()->indexOfSlot("map()");

            mapper->setMapping(obj, obj);
            mo->connect(obj, p.notifySignalIndex(), mapper, mapper_mapping_index);
        }
    }

    return objIndex;
}

void PropertyGroup::removeObject(QObject *obj)
{
    for (QSignalMapper *mapper : m_signalMapperMap) {
        mapper->removeMappings(obj);
    }

    m_objList.removeOne(obj);
}

void PropertyGroup::addProperty(const QByteArray &propertyName)
{
    m_propertyList.append(propertyName);

    QSignalMapper *mapper = new QSignalMapper(this);

    m_signalMapperMap[propertyName] = mapper;

    connect(mapper, static_cast<void (QSignalMapper::*)(QObject *)>(&QSignalMapper::mapped),
            this, &PropertyGroup::onObjectPropertyChanged);
}

void PropertyGroup::removeProperty(const QByteArray &propertyName)
{
    m_signalMapperMap.take(propertyName)->deleteLater();
}

void PropertyGroup::onObjectPropertyChanged(QObject *obj)
{
    if (QSignalMapper *mapper = qobject_cast<QSignalMapper*>(sender())) {
        const QByteArray &property_name = m_signalMapperMap.key(mapper);
        Q_ASSERT(!property_name.isEmpty());

        // 只允许一个对象的这个属性为真，其它对象必须设置为假。或许此处应该增加设置某属性所允许为真的最大数量，而不是直接定死为1
        if (obj->property(property_name.constData()).toBool()) {
            QSignalBlocker sb(m_signalMapperMap.value(property_name));
            Q_UNUSED(sb)

            for (QObject *oo : m_objList) {
                if (oo == obj)
                    continue;

                // 将其它对象的属性都都设置为假
                if (oo->property(property_name.constData()).toBool()) {
                    oo->setProperty(property_name, QVariant());
                }
            }
        }
    }
}
