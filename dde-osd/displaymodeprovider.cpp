// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "displaymodeprovider.h"

#include <DDBusSender>
#include <QSvgRenderer>
#include <DStyle>
#include <DStyleOptionButton>

#include <QGSettings>

DWIDGET_USE_NAMESPACE

const QString DisplayDBusServer = "com.deepin.daemon.Display";
const QString DisplayDBusPath = "/com/deepin/daemon/Display";
const QString DefaultCustomId = "_dde_display_config_private";
const QString schemaKey = "com.deepin.dde.control-center";
const QString schemaPath = "/com/deepin/dde/control-center/";

DisplayModeProvider::DisplayModeProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_displayInter(new Display(DisplayDBusServer,
                                 DisplayDBusPath,
                                 QDBusConnection::sessionBus(), this))
{
    if (!QGSettings::isSchemaInstalled(schemaKey.toLocal8Bit())) {
        qInfo()<<"System configuration fetch failed!";
    } else {
        m_setting = new QGSettings(schemaKey.toLocal8Bit(), schemaPath.toLocal8Bit(), this);
    }

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
    if (m_state == DisplayModeProvider::DISABLE)
        return;
    const int currentIndex = m_planItems.indexOf(m_currentPlan);
    const int itemsCount = m_planItems.length();
    m_currentPlan = m_planItems.at((currentIndex + itemsCount + 1) % itemsCount);
}

void DisplayModeProvider::sync()
{
    if (m_state == DisplayModeProvider::DISABLE)
        return;
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
        return getPlanItemIcon(index.row());
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
    if (option.state & QStyle::State_Selected && m_state != 1) {
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
    if (m_state != DisplayModeProvider::DISABLE) {
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
}

QSize DisplayModeProvider::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    if (index.row() == m_planItems.length() - 1) {
        return QSize(ItemWidth , ItemHeight);
    }
    return QSize(ItemWidth , ItemHeight + 10);
}

bool DisplayModeProvider::match(const QString &param)
{
    if (m_setting != nullptr) {
        QString status = m_setting->get("display-multiple-displays").toString();
        if (status == "Hidden")
            m_state = DisplayModeProvider::HIDE;
        if (status == "Disabled")
            m_state = DisplayModeProvider::DISABLE;
        if (status == "Enabled")
            m_state = DisplayModeProvider::ENABLE;
    }

    if (m_state == DisplayModeProvider::HIDE)
        return false;
    return AbstractOSDProvider::match(param);
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
            m_primaryScreen = m_displayInter->primary();

            m_outputNames = reply.value();
            // 按照名称排序，显示的时候VGA在前，HDMI在后
            std::sort(m_outputNames.begin(), m_outputNames.end(), std::greater<QString>());
            updatePlanItems();
            emit dataChanged();
        }
    });
}

void DisplayModeProvider::updatePlanItems()
{
    m_planItems.clear();

    m_planItems << QPair<uchar, QString>(MERGE_MODE, "");
    m_planItems << QPair<uchar, QString>(EXTEND_MODE, "");

    // 仅双屏时显示"仅X屏"
    if (m_outputNames.size() == 2) {
        for (const QString &name : m_outputNames) {
            m_planItems << QPair<uchar, QString>(SINGLE_MODE, name);
        }
    }

    auto it = std::find_if(m_planItems.begin(), m_planItems.end(), [this](const QPair<uchar, QString> &pair) {
        return (m_displayMode != SINGLE_MODE && m_displayMode == pair.first)
                || (m_displayMode == SINGLE_MODE && m_primaryScreen == pair.second);
    });

    if (it != m_planItems.end())
        m_currentPlan = *it;
}

QString DisplayModeProvider::getPlanItemName(QPair<uchar, QString> &plan) const
{
    const uchar displayMode = plan.first;

    if (displayMode == SINGLE_MODE) {
        return tr("Only on %1").arg(plan.second);
    } else if (displayMode == MERGE_MODE) {
        return tr("Duplicate");
    } else if (displayMode == EXTEND_MODE) {
        return tr("Extend");
    }

    return "";
}

QString DisplayModeProvider::getPlanItemIcon(const int index) const
{
    if (index == 0) {
        return ":/icons/display_copy.svg";
    } else if (index == 1) {
        return ":/icons/display_expansion.svg";
    } else if (index == 2) {
        return ":/icons/display_custom1.svg";
    } else if (index == 3) {
        return ":/icons/display_custom2.svg";
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

    m_primaryScreen = primary;

    updatePlanItems();
    emit dataChanged();
}
