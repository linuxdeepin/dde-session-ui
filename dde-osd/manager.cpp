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

Manager::Manager(QObject *parent)
    : QObject(parent),
      m_container(new Container),
      m_listview(new ListView),
      m_delegate(new Delegate),
      m_model(new Model(this)),
      m_currentProvider(nullptr)
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    m_listview->setItemDelegate(m_delegate);
    m_listview->setModel(m_model);

    layout->addWidget(m_listview);
    m_container->setLayout(layout);

    m_providers << new AudioProvider(this) << new BrightnessProvider(this);
}

void Manager::ShowOSD(const QString &osd)
{
    for (AbstractOSDProvider *provider : m_providers) {
        if (provider->match(osd)) {
            m_currentProvider = provider;
            connect(provider, &AbstractOSDProvider::dataChanged, this, &Manager::updateUI);
        } else {
            disconnect(provider);
        }
    }

    updateUI();
    m_container->show();
}

void Manager::updateUI()
{
    if (!m_currentProvider) return;

    m_delegate->setStyle(m_currentProvider->style());
    m_container->setFixedSize(m_currentProvider->contentSize());
    m_model->setData(m_currentProvider->data());
}
