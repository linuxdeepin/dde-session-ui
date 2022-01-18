/*
 * Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd
 *
 * Author: yinjie <yinjie@uniontech.com>
 *
 * Maintainer:
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

#include "accessibilitycheckerex.h"

/**
 * @brief addIgnoreName 添加需要忽略的控件的objectName或accessibleName，一般用于设置property无法解决的情况
 * @param name 只能是objectName或accessibleName
 */
void AccessibilityCheckerEx::addIgnoreName(const QString &name)
{
    Q_ASSERT(!name.isEmpty());

    if (!m_nameList.contains(name)) {
        m_nameList.append(name);
    }
}

/**
 * @brief addIgnoreClasses 添加需要忽略的控件的类名，一般用于addIgnoreName和设置property无法解决的情况
 * @param name 只能是objectName或accessibleName
 */
void AccessibilityCheckerEx::addIgnoreClasses(const QStringList &classes)
{
    for (const auto &className : classes) {
        if (!className.isEmpty() && !m_classes.contains(className)) {
            m_classes.append(className);
        }
    }
}


/**
 * @brief isIgnore 忽略objectName和accessibleName在m_nameList中的对象,
 * 推荐方式（按优先级排序）
 * 1.设置控件的属性：setPropert("CheckAccessibleName",false)。
 * 2.addIgnoreName
 * 3.addIgnoreClasses
 * @param role
 * @param w
 * @return true:不检查此widget，false:检查
 */
bool AccessibilityCheckerEx::isIgnore(Role role, const QWidget *w)
{
    if (!w || "VLine" == w->objectName())
        return true;

    QVariant property = w->property("CheckAccessibleName");
    if (!property.isNull() && !property.toBool())
        return true;

    if (m_nameList.contains(w->objectName()) || m_nameList.contains(w->accessibleName()))
        return true;

    if (m_classes.contains(w->metaObject()->className()))
        return true;

    return DAccessibilityChecker::isIgnore(role, w);
}
