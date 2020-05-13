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

#include "displaymodeprovider.h"
#include <DDBusSender>

DisplayModeProvider::DisplayModeProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_displayInter(new Display("com.deepin.daemon.Display",
                                 "/com/deepin/daemon/Display",
                                 QDBusConnection::sessionBus(), this))
{
    m_suitableParams << "SwitchMonitors";

    m_displayInter->setSync(false, false);

    updateOutputNames();

    connect(m_displayInter, &__Display::DisplayModeChanged,
            this, &DisplayModeProvider::displayModeChanged);
    connect(m_displayInter, &__Display::PrimaryChanged,
            this, &DisplayModeProvider::primaryChanged);
    connect(m_displayInter, &__Display::MonitorsChanged,
            this, &DisplayModeProvider::updateOutputNames);

    displayModeChanged(m_displayInter->displayMode());
    primaryChanged(m_displayInter->primary());
}

bool DisplayModeProvider::checkConditions() const
{
    return m_outputNames.length() > 1;
}

QSize DisplayModeProvider::contentSize() const
{
    return QSize(ImageTextItemWidth * m_planItems.length(), ImageTextItemHeight);
}

QListView::Flow DisplayModeProvider::flow() const
{
    return QListView::LeftToRight;
}

void DisplayModeProvider::highlightCurrent()
{
    updatePlanItems();
    emit dataChanged();
}

void DisplayModeProvider::highlightNext()
{
    const int currentIndex = m_planItems.indexOf(m_currentPlan);
    const int itemsCount = m_planItems.length();
    m_currentPlan = m_planItems.at((currentIndex + itemsCount + 1) % itemsCount);
}

void DisplayModeProvider::sync()
{
    const uchar displayMode = m_currentPlan.first;
    const QString monitorId = m_currentPlan.second;
    if (displayMode == 0) {
        if (m_displayInter->currentCustomId().isNull()) {
            DDBusSender()
            .service("com.deepin.dde.ControlCenter")
            .interface("com.deepin.dde.ControlCenter")
            .path("/com/deepin/dde/ControlCenter")
            .method(QString("ShowModule"))
            .arg(QString("display"))
            .call();
            return;
        } else {
            m_currentPlan.second = m_displayInter->currentCustomId();
        }
    }
    m_displayInter->SwitchMode(m_currentPlan.first, m_currentPlan.second);
}

void DisplayModeProvider::sync(const QModelIndex &index)
{
    m_currentPlan = m_planItems.at(index.row() % m_planItems.length());
    sync();
}

int DisplayModeProvider::currentRow()
{
    return m_planItems.indexOf(m_currentPlan);
}

int DisplayModeProvider::rowCount(const QModelIndex &) const
{
    return m_planItems.length();
}

QVariant DisplayModeProvider::data(const QModelIndex &index, int role) const
{
    QPair<uchar,QString> item = m_planItems.at(index.row());
    if (role == Qt::DecorationRole) {
        return getPlanItemIcon(item);
    }

    return getPlanItemName(item);
}

void DisplayModeProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant imageData = index.data(Qt::DecorationRole);
    QVariant textData = index.data(Qt::DisplayRole);
    if (option.state & QStyle::State_Selected) {
        DrawHelper::DrawBackground(painter, option);
        DrawHelper::DrawText(painter, option, textData.toString(), Qt::white);
    } else {
        DrawHelper::DrawText(painter, option, textData.toString());
    }

    DrawHelper::DrawImage(painter, option, imageData.toString(), true);
}

QSize DisplayModeProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(ImageTextItemWidth, ImageTextItemHeight);
}

void DisplayModeProvider::updateOutputNames()
{
    QDBusPendingCall call = m_displayInter->ListOutputNames();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher] {
        if (watcher->isError()) {
            qWarning() << "failed to get all output names" << watcher->error().message();
        } else {
            QDBusReply<QStringList> reply = watcher->reply();
            m_outputNames = reply.value();
            updatePlanItems();
            emit dataChanged();
        }
    });
}

void DisplayModeProvider::updatePlanItems()
{
    m_planItems.clear();

    m_planItems << QPair<uchar,QString>(1, "");
    m_planItems << QPair<uchar,QString>(2, "");
    m_planItems << QPair<uchar,QString>(0, m_displayInter->currentCustomId());

    for (QPair<uchar,QString> pair : m_planItems) {
        if (m_displayMode == pair.first) {
            m_currentPlan = pair;
            break;
        }
    }
}

QString DisplayModeProvider::getPlanItemName(QPair<uchar, QString> &plan) const
{
    const uchar displayMode = plan.first;
    const QString monitorId = plan.second;

    if (displayMode == 0) {
        return tr("Customize");
    } else if (displayMode == 1) {
        return tr("Duplicate");
    } else if (displayMode == 2) {
        return tr("Extend");
    }

    return "";
}

QString DisplayModeProvider::getPlanItemIcon(QPair<uchar, QString> &plan) const
{
    const uchar displayMode = plan.first;

    const bool active = plan == m_currentPlan;

    if (displayMode == 0) {
        return active ? ":/icons/OSD_custom_active.svg" : ":/icons/OSD_custom.svg";
    } else if (displayMode == 1) {
        return active ? ":/icons/OSD_copy_mode_active.svg" : ":/icons/OSD_copy_mode.svg";
    } else if (displayMode == 2) {
        return active ? ":/icons/OSD_extend_mode_active.svg" : ":/icons/OSD_extend_mode.svg";
    }
    return "";
}

void DisplayModeProvider::displayModeChanged(const uchar &mode)
{
    if (m_displayMode == mode) return;

    m_displayMode = mode;

    updatePlanItems();
    emit dataChanged();
}

void DisplayModeProvider::primaryChanged(const QString &primary)
{
    if (m_primaryScreen == primary) return;

    updatePlanItems();

    m_primaryScreen = primary;
    emit dataChanged();
}
