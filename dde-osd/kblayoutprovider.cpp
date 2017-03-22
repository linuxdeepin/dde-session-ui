#include "kblayoutprovider.h"

KBLayoutProvider::KBLayoutProvider(QObject *parent) :
    AbstractOSDProvider(parent),
    m_keyboardInter(new Keyboard("com.deepin.daemon.InputDevices",
                                 "/com/deepin/daemon/InputDevice/Keyboard",
                                 QDBusConnection::sessionBus(), this))
{
    m_suitableParams << "SwitchLayout";

    m_keyboardInter->setSync(false);
    connect(m_keyboardInter, &__Keyboard::UserLayoutListChanged,
            this, &KBLayoutProvider::userLayoutListChanged);
    connect(m_keyboardInter, &__Keyboard::CurrentLayoutChanged,
            this, &KBLayoutProvider::currentLayoutChanged);
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

bool KBLayoutProvider::checkConditions() const
{
    return m_userLayouts.length() > 1;
}

QSize KBLayoutProvider::contentSize() const
{
    int count = qMin(m_userLayouts.length(), 6);

    return QSize(TextItemWidth + 20, TextItemHeight * count + 20);
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

void KBLayoutProvider::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QVariant textData = index.data();
    if (index.row() != m_userLayouts.indexOf(m_userCurrentLayout)) {
        DrawHelper::DrawText(painter, option, textData.toString(), Qt::black, false);
    } else {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QColor::fromRgbF(0, 0, 0, 0.1));
        painter->drawRoundedRect(option.rect, 4, 4);

        DrawHelper::DrawText(painter, option, textData.toString(), ItemHighlightColor, false);
    }
}

QSize KBLayoutProvider::sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const
{
    return QSize(TextItemWidth, TextItemHeight);
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
    emit dataChanged();
}
