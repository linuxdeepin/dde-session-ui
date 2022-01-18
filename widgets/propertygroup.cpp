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
    if (m_propertyList.isEmpty())
        return -1;

    if (m_objList.contains(obj))
        return m_objList.indexOf(obj);

    const QMetaObject *mo = obj->metaObject();
    for (const QByteArray &propertyName : m_propertyList) {
        // sender是否包含此属性
        int propertyIndex = mo->indexOfProperty(propertyName);
        if (-1 == propertyIndex)
            continue;

        QMetaProperty p = mo->property(propertyIndex);
        Q_ASSERT(p.isReadable() && p.isWritable() && p.hasNotifySignal());

        // 如果目前已存在对象的这个属性为真，则将新加入对象的此属性设置为假
        const bool exist = std::any_of(m_objList.begin(), m_objList.end(), [p, obj](const QObject *oo) { 
            return (oo != obj && oo->property(p.name()).toBool()); 
        });
        if (exist)
            p.write(obj, QVariant());

        // 与onObjectPropertyChanged函数进行绑定
        const int index = this->metaObject()->indexOfSlot("onObjectPropertyChanged()");
        if (-1 == index)
            return -1;

        mo->connect(obj, p.notifySignalIndex(), this, index);
    }

    m_objList.append(obj);
    connect(obj, &QObject::destroyed, this, &PropertyGroup::removeObject);

    return m_objList.count() - 1;
}

void PropertyGroup::removeObject(QObject *obj)
{
    m_objList.removeAll(obj);
}

void PropertyGroup::addProperty(const QByteArray &propertyName)
{
    if (!m_propertyList.contains(propertyName))
        m_propertyList.append(propertyName);
}

void PropertyGroup::removeProperty(const QByteArray &propertyName)
{
    m_propertyList.removeAll(propertyName);
}

void PropertyGroup::onObjectPropertyChanged()
{
    QObject *senderObj = sender();
    if (!m_objList.contains(senderObj))
        return;

    const QMetaObject *mo = senderObj->metaObject();
    QMetaMethod senderSignal = mo->method(senderSignalIndex());

    for (const QByteArray &propertyName : m_propertyList) {
        // sender是否包含此属性
        int propertyIndex = mo->indexOfProperty(propertyName);
        if (propertyIndex == -1)
            continue;

        // 判断发送变更信号的是否是此属性
        QMetaProperty property = mo->property(propertyIndex);
        if (!property.hasNotifySignal() || property.notifySignal() != senderSignal)
            return;

        // 将其它对象的属性都都设置为假
        if (senderObj->property(propertyName.constData()).toBool()) {
            for (QObject *obj : m_objList) {
                if (obj == senderObj)
                    continue;

                if (obj->property(propertyName.constData()).toBool()) {
                    obj->setProperty(propertyName, QVariant());
                }
            }
        }
    }
}
