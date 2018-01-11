#include "processinfomodel.h"

#include <QDebug>
#include <QIcon>
#include <QFile>
#include <QRegularExpression>

inline QString formatMem(const unsigned mem_bytes)
{
    return QString("%1M").arg(mem_bytes / 1024 / 1024);
}

inline QString appName(const QString &desktop)
{
    if (!desktop.endsWith(".desktop"))
        return QString();

    const int start = desktop.lastIndexOf('/') + 1;
    const int end = desktop.lastIndexOf(".desktop");

    return desktop.midRef(start, end - start).toString();
}

QPixmap appIcon(const int size, const QString &desktop)
{
    static QIcon defaultIcon = QIcon::fromTheme("application-x-desktop");

    QFile f(desktop);
    if (f.open(QIODevice::ReadOnly))
    {
        const QString &content = f.readAll();

        QRegularExpression iconRegex("Icon=(.*)$");
        const auto &match = iconRegex.match(content);
        if (match.hasMatch())
        {
            const QString &icon = match.captured(1);

            if (QFile(icon).exists())
                return QIcon(icon).pixmap(size, size);

            return QIcon::fromTheme(icon, defaultIcon).pixmap(size, size);
        }
    }

    return QIcon::fromTheme(appName(desktop), defaultIcon).pixmap(size, size);
}

ProcessInfoModel::ProcessInfoModel(QObject *parent)
    : QAbstractTableModel(parent)

    , m_processInfos(new ProcessInfoManager)
{
    connect(m_processInfos, &ProcessInfoManager::processInfoListChanged, this, [=] { emit layoutChanged(); });
}

int ProcessInfoModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return m_processInfos->processInfoList.size();
}

int ProcessInfoModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    return COLUMN_NUMS;
}

QVariant ProcessInfoModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case DisplayRole:
        switch (index.column())
        {
        case COLUMN_FREE_BTN:
            return QString(tr("Release"));
        case COLUMN_MEM:
            return formatMem(m_processInfos->processInfoList[index.row()].total_mem_bytes);
        case COLUMN_NAME:
            return m_processInfos->processInfoList[index.row()].app_name;
        default:;
        }
        break;
    case IconRole:
    {
        const QString &desktop = m_processInfos->processInfoList[index.row()].desktop;
        switch (index.column())
        {
        case COLUMN_ICON:
            return appIcon(24, desktop);
        case COLUMN_FREE_BTN:
            return appIcon(48, desktop);
        default:;
        }
    }
        break;
    case TextAlignmentRole:
        switch (index.column())
        {
        case COLUMN_MEM:    return int(Qt::AlignRight | Qt::AlignVCenter);
        default:            return int(Qt::AlignLeft | Qt::AlignVCenter);
        }
    case StateRole:
        return m_pressedIndex == index;
    case PidListRole:
        return m_processInfos->processInfoList[index.row()].pid_list;
    case TabIdRole:
        return m_processInfos->processInfoList[index.row()].id;
    default:;
    }

    return QVariant();
}

void ProcessInfoModel::clearPressed()
{
    const QModelIndex idx = m_pressedIndex;
    m_pressedIndex = QModelIndex();

    emit dataChanged(idx, idx);
}

bool ProcessInfoModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)
    Q_UNUSED(value)

    m_pressedIndex = index;

    return true;
}
