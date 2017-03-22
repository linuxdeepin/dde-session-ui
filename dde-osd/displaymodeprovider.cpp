#include "displaymodeprovider.h"

DisplayModeProvider::DisplayModeProvider(QObject *parent)
    : AbstractOSDProvider(parent),
      m_displayInter(new Display("com.deepin.daemon.Display",
                                 "/com/deepin/daemon/Display",
                                 QDBusConnection::sessionBus(), this))
{
    m_suitableParams << "SwitchMonitors";

    m_displayInter->setSync(false);

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

    m_displayInter->SwitchMode(displayMode, monitorId);
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

    DrawHelper::DrawImage(painter, option, QPixmap(imageData.toString()), true);

    const int currentIndex = m_planItems.indexOf(m_currentPlan);
    if (currentIndex != index.row()) {
        DrawHelper::DrawText(painter, option, textData.toString());
    } else {
        DrawHelper::DrawText(painter, option, textData.toString(), ItemHighlightColor);
    }
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
            emit dataChanged();
        }
    });
}

void DisplayModeProvider::updatePlanItems()
{
    m_planItems.clear();

    m_planItems << QPair<uchar,QString>(1, "");
    m_planItems << QPair<uchar,QString>(2, "");
    for (const QString &output : m_outputNames) {
        m_planItems << QPair<uchar,QString>(3, output);
    }

    m_currentPlan = QPair<uchar,QString>(m_displayMode, "");
    if (m_displayMode == 3) {
        m_currentPlan.second = m_primaryScreen;
    }
}

QString DisplayModeProvider::getPlanItemName(QPair<uchar, QString> &plan) const
{
    const uchar displayMode = plan.first;
    const QString monitorId = plan.second;

    if (displayMode == 1) {
        return tr("Duplicate");
    } else if (displayMode == 2) {
        return tr("Extend");
    } else if (displayMode == 3) {
        return monitorId;
    }

    return "";
}

QString DisplayModeProvider::getPlanItemIcon(QPair<uchar, QString> &plan) const
{
    const uchar displayMode = plan.first;
    const QString monitorId = plan.second;

    const bool active = plan == m_currentPlan;

    if (displayMode == 1) {
        return active ? ":/icons/OSD_copy_mode_active.png" : ":/icons/OSD_copy_mode.png";
    } else if (displayMode == 2) {
        return active ? ":/icons/OSD_extend_mode_active.png" : ":/icons/OSD_extend_mode.png";
    } else if (displayMode == 3) {
        const int index = m_outputNames.indexOf(monitorId) + 1;
        return QString(":/icons/OSD_only%1%2.png").arg(index).arg(active ? "_active" : "");
    }

    return "";
}

void DisplayModeProvider::displayModeChanged(const uchar &mode)
{
    if (m_displayMode == mode) return;

    updatePlanItems();

    m_displayMode = mode;
    emit dataChanged();
}

void DisplayModeProvider::primaryChanged(const QString &primary)
{
    if (m_primaryScreen == primary) return;

    updatePlanItems();

    m_primaryScreen = primary;
    emit dataChanged();
}
