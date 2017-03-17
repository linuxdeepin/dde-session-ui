#include "manager.h"

#include <QHBoxLayout>
#include <QDebug>

#include "container.h"
#include "listview.h"
#include "delegate.h"
#include "model.h"
#include "common.h"

#define Pair(x, y) QPair<QString, QString>(x, y)

Manager::Manager(QObject *parent)
    : QObject(parent),
      m_container(new Container),
      m_listview(new ListView),
      m_delegate(new Delegate),
      m_model(new Model(this)),
      m_style(StyleImage),

      m_data(new DataWrapper(this)),
      m_audioInter(new com::deepin::daemon::Audio("com.deepin.daemon.Audio", "/com/deepin/daemon/Audio", QDBusConnection::sessionBus(), this)),
      m_sinkInter(nullptr),
      m_displayInter(new com::deepin::daemon::Display("com.deepin.daemon.Display", "/com/deepin/daemon/Display", QDBusConnection::sessionBus(), this))
{
    m_audioInter->setSync(false);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);

    m_listview->setItemDelegate(m_delegate);
    m_listview->setModel(m_model);

    layout->addWidget(m_listview);
    m_container->setLayout(layout);

    initData();
    connect(m_data, &DataWrapper::dataChanged, this, &Manager::updateUI);
}

void Manager::ShowOSD(const QString &osd)
{
    m_param = osd;

    if (osd.startsWith("Audio") || osd.startsWith("Brightness")) {
        setStyle(StyleImageProg);
    } else if (osd.endsWith("On") || osd.endsWith("Off")) {
        setStyle(StyleImage);
    } else if (osd.startsWith("SwitchWM")) {
        setStyle(StyleImageText);
    }

    updateUI();
    m_container->show();
}

void Manager::setStyle(const Style &style)
{
    m_style = style;
    m_delegate->setStyle(style);
    setupData();
    setupSize();
}

void Manager::initData()
{
    connect(m_audioInter, &__Audio::DefaultSinkChanged, this, &Manager::defaultSinkChanged);
    defaultSinkChanged(m_audioInter->defaultSink());

    connect(m_displayInter, &__Display::BrightnessChanged, this, &Manager::brightnessChanged);
    brightnessChanged(m_displayInter->brightness());
}

void Manager::updateUI()
{
    setupSize();
    setupData();
}

void Manager::setupSize()
{
    switch (m_style) {
    case StyleImage:
    case StyleImageText:
    case StyleImageProg:
        m_container->setFixedSize(ImageTextItemWidth, ImageTextItemHeight);
        break;
    default:
        m_container->setFixedSize(ImageTextItemWidth, ImageTextItemHeight);
        break;
    }
}

void Manager::setupData()
{
    QList<QPair<QString, QString>> value;

    const QString path = getCurrentPixmapPath();
    if (m_param.startsWith("Audio")) {
        const double progress = m_data->mute() ? 0 : m_data->volume();
        value << Pair(path, QString::number(progress));
    } else if (m_param.startsWith("Brightness")) {
        value << Pair(path, QString::number(m_data->brightness()));
    }

    m_model->setData(value);
}

QString Manager::getCurrentPixmapPath()
{
    if (m_param.startsWith("Audio")) {
        if (m_data->mute()) {
            return ":/icons/OSD_mute.png";
        }

        const int level = m_data->volume() > 0.6 ? 3 : m_data->volume() > 0.3 ? 2 : 1;
        return QString(":/icons/OSD_volume_%1.png").arg(level);
    } else if (m_param.startsWith("Brightness")) {
        return ":/icons/OSD_light.png";
    }

    return "";
}

void Manager::defaultSinkChanged(const QDBusObjectPath &path)
{
    const QString pathStr = path.path();
    if (!pathStr.isEmpty()) {
        if (m_sinkInter) {
            m_sinkInter->deleteLater();
            m_sinkInter = nullptr;
        }

        m_sinkInter = new com::deepin::daemon::audio::Sink("com.deepin.daemon.Audio", pathStr, QDBusConnection::sessionBus(), this);
        m_sinkInter->setSync(false);

        connect(m_sinkInter, &__Sink::VolumeChanged, m_data, &DataWrapper::setVolume);
        connect(m_sinkInter, &__Sink::MuteChanged, m_data, &DataWrapper::setMute);
        m_data->setVolume(m_sinkInter->volume());
        m_data->setMute(m_sinkInter->mute());
    }
}

void Manager::brightnessChanged(const BrightnessMap &brightness)
{
    m_data->setBrightness(brightness.values().at(0));
}

DataWrapper::DataWrapper(QObject *parent) :
    QObject(parent),
    m_volume(0),
    m_mute(false)
{

}

double DataWrapper::volume() const
{
    return m_volume;
}

void DataWrapper::setVolume(double volume)
{
    qDebug() << "set volume" << volume;
    if (volume == m_volume) return;

    m_volume = volume;
    emit dataChanged();
}

bool DataWrapper::mute() const
{
    return m_mute;
}

void DataWrapper::setMute(bool mute)
{
    qDebug() << "set mute" << mute;
    if (mute == m_mute) return;

    m_mute = mute;
    emit dataChanged();
}

double DataWrapper::brightness() const
{
    return m_brightness;
}

void DataWrapper::setBrightness(double brightness)
{
    qDebug() << "set brightness" << brightness;
    if (brightness == m_brightness) return;

    m_brightness = brightness;
    emit dataChanged();
}
