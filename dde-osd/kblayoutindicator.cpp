#include "kblayoutindicator.h"

#include <QDBusConnection>
#include <QPainter>

#include <QMouseEvent>

static QString nameOfLayout(const QString &layout)
{
    QString ret( layout );
    return ret.remove(";");
}

DataWrapper::DataWrapper(KeyboardInterface *kinter, QObject *parent) :
    QObject(parent),
    m_keyboardInter(kinter)
{
    m_keyboardInter->setSync(false);

    initLayoutListAll();

    connect(m_keyboardInter, &__Keyboard::UserLayoutListChanged,
            this, &DataWrapper::setLayoutList);
    connect(m_keyboardInter, &__Keyboard::CurrentLayoutChanged,
            this, &DataWrapper::setCurrentLayout);

    setLayoutList(m_keyboardInter->userLayoutList());
    setCurrentLayout(m_keyboardInter->currentLayout());
}

QString DataWrapper::currentLayout() const
{
    return m_currentLayout;
}

void DataWrapper::setCurrentLayout(const QString &currentLayout)
{
    qDebug() << "current layout changed to: " << currentLayout;

    if (currentLayout == m_currentLayout)
        return;

    m_currentLayout = currentLayout;
    emit currentLayoutChanged(m_currentLayout);
}

QStringList DataWrapper::layoutList() const
{
    return m_layoutList;
}

void DataWrapper::setLayoutList(const QStringList &layoutList)
{
    qDebug() << "user layouts changed to: " << layoutList;

    if (layoutList == m_layoutList)
        return;

    m_layoutList = layoutList;
    emit layoutListChanged(layoutList);
}

void DataWrapper::initLayoutListAll()
{
    QDBusPendingCall call = m_keyboardInter->LayoutList();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, call] {
        if (call.isError()) {
            qWarning() << "failed to get all keyboard list: " << call.error().message();
        } else {
            QDBusReply<KeyboardLayoutList> reply = call.reply();
            m_layoutListAll = reply.value();
        }
    });
}

QString DataWrapper::localizedNameOfLayout(const QString &layout) const
{
    return m_layoutListAll.value(layout);
}

KBLayoutIndicator::KBLayoutIndicator(QWidget *parent)
    : QSystemTrayIcon(parent),
      m_keyboardInter(new KeyboardInterface("com.deepin.daemon.InputDevices",
                                            "/com/deepin/daemon/InputDevice/Keyboard",
                                            QDBusConnection::sessionBus(),
                                            this)),
      m_data(new DataWrapper(m_keyboardInter, this)),
      m_menu(new QMenu),
      m_addLayoutAction(nullptr)
{
    updateIcon();
    updateMenu();

    connect(m_data, &DataWrapper::layoutListChanged,
            this, &KBLayoutIndicator::handleDataChanged);
    connect(m_data, &DataWrapper::currentLayoutChanged,
            this, &KBLayoutIndicator::handleDataChanged);

    connect(m_menu, &QMenu::triggered,
            this, &KBLayoutIndicator::handleActionTriggered);

    connect(this, &KBLayoutIndicator::activated,
            this, &KBLayoutIndicator::handleActivated);
}

void KBLayoutIndicator::updateMenu()
{
    if (m_menu->actions().length())
        m_menu->clear();

    setContextMenu(m_menu);

    for (const QString &layout : m_data->layoutList()) {
        const QString layoutName = nameOfLayout(layout);
        const QString layoutLocalizedName = m_data->localizedNameOfLayout(layout);
        const QString text = QString("%1 (%2)").arg(layoutLocalizedName).arg(layoutName);

        QAction *action = new QAction(text, m_menu);
        action->setObjectName(layout);
        action->setCheckable(true);
        action->setChecked(layout == m_data->currentLayout());
        m_menu->addAction(action);
    }

    m_menu->addSeparator();

    m_addLayoutAction = new QAction(tr("Add keyboard layout"), m_menu);

    m_menu->addAction(m_addLayoutAction);
}

void KBLayoutIndicator::updateIcon()
{
    const QString layout = nameOfLayout(m_data->currentLayout());

    QPixmap pix(16, 16);
    pix.fill(Qt::transparent);

    QPainter pa(&pix);
    pa.setRenderHints(pa.renderHints() | QPainter::Antialiasing);
    pa.setPen(Qt::white);

    QFont font = pa.font();
    font.setPixelSize(layout.length() > 2 ? 10 : 15);
    font.setWeight(QFont::Bold);
    pa.setFont(font);

    QTextOption op;
    op.setAlignment(Qt::AlignCenter);

    QRect r( pix.rect() );
    r.adjust(0, -2, 0, 0); // make the text looks more center aligned.
    pa.drawText(r, layout, op);

    setIcon(QIcon(pix));
}

void KBLayoutIndicator::switchToNextLayout()
{
    const QString layout = m_data->currentLayout();
    const QStringList layouts = m_data->layoutList();

    const int currentIdx = layouts.indexOf(layout);
    const QString targetLayout = layouts.at((currentIdx + 1) % layouts.length());

    m_keyboardInter->setCurrentLayout(targetLayout);
}

void KBLayoutIndicator::handleDataChanged()
{
    if (m_data->layoutList().length() < 2)
        return setVisible(false);

    updateIcon();
    updateMenu();
    show();
}

void KBLayoutIndicator::handleActionTriggered(QAction *action)
{
    if (action == m_addLayoutAction) {
        QProcess::startDetached("dbus-send --print-reply --dest=com.deepin.dde.ControlCenter "
                                "/com/deepin/dde/ControlCenter "
                                "com.deepin.dde.ControlCenter.ShowModule string:keyboard");
    }

    const QString layout = action->objectName();
    if (m_data->layoutList().contains(layout)) {
        m_keyboardInter->setCurrentLayout(layout);
    }
}

void KBLayoutIndicator::handleActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        switchToNextLayout();
    }
}

