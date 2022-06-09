/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
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

#include "kblayoutprovider.h"

#include <QApplication>

KBLayoutProvider::KBLayoutProvider(QObject *parent)
    : AbstractOSDProvider(parent)
    , m_keyboardInter(new Keyboard("com.deepin.daemon.InputDevices",
                                 "/com/deepin/daemon/InputDevice/Keyboard",
                                 QDBusConnection::sessionBus(), this))
{
    m_suitableParams << "SwitchLayout";

    m_keyboardInter->setSync(false, false);

    auto keyboardIsValid = [ = ](bool isvalid) {
        if (isvalid) {
            userLayoutListChanged(m_keyboardInter->userLayoutList());
            currentLayoutChanged(m_keyboardInter->currentLayout());

            QDBusPendingCall call = m_keyboardInter->LayoutList();
            QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
            connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher] {
                if (watcher->isError()) {
                    qWarning() << "failed to get all layout lists" << watcher->error();
                } else {
                    QDBusReply<KeyboardLayoutList> reply = watcher->reply();
                    m_database = reply.value();
                }
            });
        }
    };

    connect(m_keyboardInter, &__Keyboard::UserLayoutListChanged,
            this, &KBLayoutProvider::userLayoutListChanged);
    connect(m_keyboardInter, &__Keyboard::CurrentLayoutChanged,
            this, &KBLayoutProvider::currentLayoutChanged);
    connect(m_keyboardInter, &__Keyboard::serviceValidChanged,
            this, keyboardIsValid);

    keyboardIsValid(m_keyboardInter->isValid());
}

bool KBLayoutProvider::checkConditions() const
{
    return m_userLayouts.length() > 1;
}

QSize KBLayoutProvider::contentSize() const
{
    int count = qMin(m_userLayouts.length(), 6);

    // Find the langest layout and set max width
    QStringList list;

    for (const QString &s : m_userLayouts) {
        list << m_database.value(s, s);
    }

    std::sort(list.begin(), list.end(), [ = ](const QString & s1, const QString & s2) {
        return s1.length() > s2.length();
    });

    const QFont appNamefont(qApp->font());
    const QFontMetrics fm(appNamefont);

    return QSize(qMax(fm.width(list.first()), TextItemWidth) + 30, TextItemHeight * count + 20);
}

QMargins KBLayoutProvider::contentMargins() const
{
    return QMargins(10, 10, 10, 10);
}

void KBLayoutProvider::highlightCurrent()
{
    currentLayoutChanged(m_keyboardInter->currentLayout());
}

void KBLayoutProvider::highlightNext()
{
    const int index = m_userLayouts.indexOf(m_userCurrentLayout);
    const int count = m_userLayouts.length();
    m_userCurrentLayout = m_userLayouts.at((index + count + 1) % count);
}

void KBLayoutProvider::sync()
{
    m_keyboardInter->setCurrentLayout(m_userCurrentLayout);
}

int KBLayoutProvider::rowCount(const QModelIndex &) const
{
    return m_userLayouts.length();
}

QVariant KBLayoutProvider::data(const QModelIndex &index, int) const
{
    return describeLayout(m_userLayouts.at(index.row()));
}

int KBLayoutProvider::currentIndex() const
{
    return  m_userLayouts.indexOf(m_userCurrentLayout);
}

void KBLayoutProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant textData = index.data();
    QPalette palette;

    const QString text = option.fontMetrics.elidedText(textData.toString(), Qt::ElideRight, option.rect.width());

    if (index.row() != m_userLayouts.indexOf(m_userCurrentLayout)) {
        DrawHelper::DrawText(painter, option, text, palette.color(QPalette::BrightText), false);
    } else {
        QColor brushCorlor;
        brushCorlor = palette.color(QPalette::Base);
        brushCorlor.setAlphaF(0.4);

        painter->setPen(Qt::NoPen);
        painter->setBrush(brushCorlor);
        painter->drawRoundedRect(option.rect, 6, 6);

        DrawHelper::DrawText(painter, option, text, palette.color(QPalette::Highlight), false);
    }
}

QSize KBLayoutProvider::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(qMax(TextItemWidth, option.fontMetrics.width(index.data().toString())), TextItemHeight);
}

QString KBLayoutProvider::describeLayout(const QString &layout) const
{
    return m_database.value(layout, layout);
}

void KBLayoutProvider::currentLayoutChanged(const QString &layout)
{
    if (m_userCurrentLayout == layout) return;

    m_userCurrentLayout = layout;
    emit dataChanged();
}

void KBLayoutProvider::userLayoutListChanged(const QStringList &layouts)
{
    m_userLayouts = layouts;
    QDBusReply<KeyboardLayoutList> reply = m_keyboardInter->LayoutList();
    m_database = reply.value();
    emit dataChanged();
}
