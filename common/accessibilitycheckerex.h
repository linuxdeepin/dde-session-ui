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

#ifndef ACCESSIBILITYCHECKEREX_H
#define ACCESSIBILITYCHECKEREX_H

#include <QObject>
#include <QWidget>

#include <DAccessibilityChecker>

DWIDGET_USE_NAMESPACE

class AccessibilityCheckerEx : public DAccessibilityChecker
{
    Q_OBJECT
public:
    void addIgnoreName(const QString &name);
    void addIgnoreClasses(const QStringList &classes);

protected:
    virtual bool isIgnore(Role role, const QWidget *w) override ;

private:
    QStringList m_nameList;
    QStringList m_classes;
};

#endif // ACCESSIBILITYCHECKEREX_H
