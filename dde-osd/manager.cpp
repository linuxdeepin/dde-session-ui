#include "manager.h"

#include <QHBoxLayout>
#include <QDebug>

#include "container.h"
#include "listview.h"
#include "delegate.h"
#include "model.h"
#include "common.h"

#include "audioprovider.h"
#include "brightnessprovider.h"
#include "kblayoutprovider.h"
#include "displaymodeprovider.h"
#include "indicatorprovider.h"
#include "wmstateprovider.h"

Manager::Manager(QObject *parent)
    : QObject(parent),
      m_container(new Container),
      m_listview(new ListView),
      m_delegate(new Delegate),
      m_model(new Model(this)),
      m_currentProvider(nullptr),
      m_kbLayoutProvider(new KBLayoutProvider(this)),
      m_timer(new QTimer)
{
    m_timer->setSingleShot(true);
    m_timer->setInterval(2000);

    m_listview->setItemDelegate(m_delegate);
    m_listview->setModel(m_model);
    m_container->setContent(m_listview);

    m_providers << new AudioProvider(this) << new BrightnessProvider(this);
    m_providers << m_kbLayoutProvider << new DisplayModeProvider(this);
    m_providers << new IndicatorProvider(this) << new WMStateProvider(this);

    connect(m_timer, &QTimer::timeout, this, &Manager::doneSetting);
}

void Manager::ShowOSD(const QString &osd)
{
    qDebug() << "show osd" << osd;

    if (osd == "DirectSwitchLayout") {
        m_kbLayoutProvider->highlightNext();
        m_kbLayoutProvider->sync();
        return;
    }

    if (osd == "SwitchLayoutDone") {
        doneSetting();
        return;
    }

    bool repeat = false;
    for (AbstractOSDProvider *provider : m_providers) {
        if (provider->match(osd)) {
            repeat = (m_currentProvider == provider);
            m_currentProvider = provider;
            connect(provider, &AbstractOSDProvider::dataChanged, this, &Manager::updateUI);
        } else {
            disconnect(provider);
        }
    }

    if (m_currentProvider) {
        updateUI();
        if (repeat && m_container->isVisible()) {
            QModelIndex currentIndex = m_listview->currentIndex();
            if (currentIndex.row() < 0)
                currentIndex = m_listview->model()->index(0, 0);

            QModelIndex targetIndex;
            if (currentIndex.row() + 1 >= m_listview->model()->rowCount(QModelIndex())) {
                targetIndex = m_listview->model()->index(0, 0);
            } else {
                targetIndex = currentIndex.sibling(currentIndex.row() + 1, 0);
            }

            m_listview->setCurrentIndex(targetIndex);
            m_currentProvider->highlightNext();

        } else {
            m_currentProvider->highlightCurrent();
        }

        if (m_currentProvider->checkConditions()) {
            m_container->show();
            m_timer->start();
        }
    }
}

void Manager::updateUI()
{
    if (!m_currentProvider) return;

    if (m_model->provider() != m_currentProvider)
        m_model->setProvider(m_currentProvider);

    m_delegate->setProvider(m_currentProvider);
    m_listview->setFlow(m_currentProvider->flow());
    m_container->setContentsMargins(m_currentProvider->contentMargins());
    m_container->setFixedSize(m_currentProvider->contentSize());
    m_container->moveToCenter();
}

void Manager::doneSetting()
{
    m_timer->stop();
    m_container->hide();
    if (m_currentProvider) {
        m_currentProvider->sync();
        m_currentProvider = nullptr;
    }
}
