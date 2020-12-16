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
#include <QSvgRenderer>
#include <DStyle>
#include <DStyleOptionButton>

DWIDGET_USE_NAMESPACE

const QString DisplayDBusServer = "com.deepin.daemon.Display";
const QString DisplayDBusPath = "/com/deepin/daemon/Display";
const QString DefaultCustomId = "_dde_display_config_private";

DisplayModeProvider::DisplayModeProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_displayInter(new Display(DisplayDBusServer,
                                 DisplayDBusPath,
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

QMargins DisplayModeProvider::contentMargins() const
{
    return QMargins(10, 10, 10, 10);
}

QSize DisplayModeProvider::contentSize() const
{
    return QSize(ItemWidth, (ItemHeight + 10) * m_planItems.length() + 10 );
}

QListView::Flow DisplayModeProvider::flow() const
{
    return QListView::TopToBottom;
}

void DisplayModeProvider::highlightCurrent()
{
    updatePlanItems();
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
        DDBusSender()
        .service("com.deepin.dde.ControlCenter")
        .interface("com.deepin.dde.ControlCenter")
        .path("/com/deepin/dde/ControlCenter")
        .method(QString("ShowPage"))
        .arg(QString("display"))
        .arg(QString("Multiple Displays"))
        .call();
        if (!m_displayInter->currentCustomId().isNull()) {
            m_currentPlan.second = m_displayInter->currentCustomId();
        } else { // 如果没有自定义模式，同控制中心设置默认自定义模式
            m_currentPlan.second = DefaultCustomId;
        }
    }
    m_displayInter->SwitchMode(m_currentPlan.first,  m_currentPlan.second);
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
    painter->setRenderHint(QPainter::Antialiasing, true);

    QVariant imageData = index.data(Qt::DecorationRole);
    QVariant textData = index.data(Qt::DisplayRole);
    const QRect rect(option.rect);
    QPalette palette;
    bool checkState = false;

    QRect backRect;
    if (index.row() == m_planItems.length() - 1) {
        backRect = rect;
    } else {
        backRect = rect.marginsRemoved(QMargins(0, 0, 0, 10));
    }

    QColor textCorlor;

    QColor brushCorlor;
    brushCorlor = palette.color(QPalette::Base);
    // 绘制背景
    painter->setPen(Qt::NoPen);
    if (option.state & QStyle::State_Selected) {
        checkState = true;
        brushCorlor.setAlphaF(0.4);
        textCorlor = palette.color(QPalette::Highlight);
    } else {
        checkState = false;
        brushCorlor.setAlphaF(0.1);
        textCorlor = palette.color(QPalette::BrightText);
    }

    painter->setBrush(brushCorlor);
    painter->drawRoundedRect(backRect, 8, 8);

    // 绘制图标
    QSvgRenderer renderer(imageData.toString());
    QRect imageRect = QRect(QPoint(backRect.x() + ICON_HSPACE,
                                   backRect.y() + ((backRect.height() - IconSize) / 2)),
                                   QSize(IconSize, IconSize));
    renderer.render(painter, imageRect);

    // 绘制文字
    QTextOption opt;
    QRect textRect = backRect.marginsRemoved(QMargins(IconSize + ICON_HSPACE * 2, 0, 0, 0));
    opt.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    painter->setPen(textCorlor);
    painter->drawText(textRect, textData.toString(), opt);

    // 画复选框
    if (checkState) {
        QRect checkButtonRect = backRect.marginsRemoved(QMargins(backRect.width() - CHECK_ICON_SIZE - CHECK_ICON_HSPACE,
                                                        (backRect.height() - CHECK_ICON_SIZE) / 2, CHECK_ICON_HSPACE,
                                                        (backRect.height() - CHECK_ICON_SIZE) / 2));
        painter->setPen(Qt::NoPen);
        painter->setBrush(textCorlor);
        painter->drawEllipse(checkButtonRect);

        QPen pen(Qt::white, CHECK_ICON_SIZE / 100.0 * 6.20, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
        painter->setPen(pen);

        QPointF points[3] = {
            QPointF(CHECK_ICON_SIZE / 100.0 * 32 + checkButtonRect.x(),  CHECK_ICON_SIZE / 100.0 * 57 + checkButtonRect.y()),
            QPointF(CHECK_ICON_SIZE / 100.0 * 45 + checkButtonRect.x(),  CHECK_ICON_SIZE / 100.0 * 70 + checkButtonRect.y()),
            QPointF(CHECK_ICON_SIZE / 100.0 * 75 + checkButtonRect.x(),  CHECK_ICON_SIZE / 100.0 * 35 + checkButtonRect.y())
        };

        painter->drawPolyline(points, 3);
    }
}

QSize DisplayModeProvider::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    if (index.row() == m_planItems.length() - 1) {
        return QSize(ItemWidth , ItemHeight);
    }
    return QSize(ItemWidth , ItemHeight + 10);
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
            m_displayMode = m_displayInter->displayMode();
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

    if (displayMode == 0) {
        return ":/icons/display_custom.svg";
    } else if (displayMode == 1) {
        return ":/icons/display_copy.svg";
    } else if (displayMode == 2) {
        return ":/icons/display_expansion.svg";
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
