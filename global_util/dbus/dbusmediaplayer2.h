// SPDX-FileCopyrightText: 2015 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef DBUSMEDIAPLAYER2_H_1446777070
#define DBUSMEDIAPLAYER2_H_1446777070

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

typedef QMap<QString, QVariant> Dict;
Q_DECLARE_METATYPE(Dict)

/*
 * Proxy class for interface org.mpris.MediaPlayer2.Player
 */
class DBusMediaPlayer2: public QDBusAbstractInterface
{
    Q_OBJECT

    Q_SLOT void __propertyChanged__(const QDBusMessage& msg)
    {
        QList<QVariant> arguments = msg.arguments();
        if (3 != arguments.count())
            return;
        QString interfaceName = msg.arguments().at(0).toString();
        if (interfaceName !="org.mpris.MediaPlayer2.Player")
            return;
        QVariantMap changedProps = qdbus_cast<QVariantMap>(arguments.at(1).value<QDBusArgument>());
        QStringList keys = changedProps.keys();
        foreach(const QString &prop, keys) {
        const QMetaObject* self = metaObject();
            for (int i=self->propertyOffset(); i < self->propertyCount(); ++i) {
                QMetaProperty p = self->property(i);
                if (p.name() == prop) {
 	            Q_EMIT p.notifySignal().invoke(this);
                }
            }
        }
   }
public:
    static inline const char *staticInterfaceName()
    { return "org.mpris.MediaPlayer2.Player"; }

public:
    DBusMediaPlayer2(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~DBusMediaPlayer2();

    Q_PROPERTY(bool CanControl READ canControl NOTIFY CanControlChanged)
    inline bool canControl() const
    { return qvariant_cast< bool >(property("CanControl")); }

    Q_PROPERTY(bool CanGoNext READ canGoNext NOTIFY CanGoNextChanged)
    inline bool canGoNext() const
    { return qvariant_cast< bool >(property("CanGoNext")); }

    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious NOTIFY CanGoPreviousChanged)
    inline bool canGoPrevious() const
    { return qvariant_cast< bool >(property("CanGoPrevious")); }

    Q_PROPERTY(bool CanPause READ canPause NOTIFY CanPauseChanged)
    inline bool canPause() const
    { return qvariant_cast< bool >(property("CanPause")); }

    Q_PROPERTY(bool CanPlay READ canPlay NOTIFY CanPlayChanged)
    inline bool canPlay() const
    { return qvariant_cast< bool >(property("CanPlay")); }

    Q_PROPERTY(bool CanSeek READ canSeek NOTIFY CanSeekChanged)
    inline bool canSeek() const
    { return qvariant_cast< bool >(property("CanSeek")); }

    Q_PROPERTY(QString LoopStatus READ loopStatus WRITE setLoopStatus NOTIFY LoopStatusChanged)
    inline QString loopStatus() const
    { return qvariant_cast< QString >(property("LoopStatus")); }
    inline void setLoopStatus(const QString &value)
    { setProperty("LoopStatus", QVariant::fromValue(value)); }

    Q_PROPERTY(double MaximumRate READ maximumRate NOTIFY MaximumRateChanged)
    inline double maximumRate() const
    { return qvariant_cast< double >(property("MaximumRate")); }

    Q_PROPERTY(Dict Metadata READ metadata NOTIFY MetadataChanged)
    inline Dict metadata() const
    { return qvariant_cast< Dict >(property("Metadata")); }

    Q_PROPERTY(double MinimumRate READ minimumRate NOTIFY MinimumRateChanged)
    inline double minimumRate() const
    { return qvariant_cast< double >(property("MinimumRate")); }

    Q_PROPERTY(QString PlaybackStatus READ playbackStatus NOTIFY PlaybackStatusChanged)
    inline QString playbackStatus() const
    { return qvariant_cast< QString >(property("PlaybackStatus")); }

    Q_PROPERTY(qlonglong Position READ position NOTIFY PositionChanged)
    inline qlonglong position() const
    { return qvariant_cast< qlonglong >(property("Position")); }

    Q_PROPERTY(double Rate READ rate WRITE setRate NOTIFY RateChanged)
    inline double rate() const
    { return qvariant_cast< double >(property("Rate")); }
    inline void setRate(double value)
    { setProperty("Rate", QVariant::fromValue(value)); }

    Q_PROPERTY(bool Shuffle READ shuffle WRITE setShuffle NOTIFY ShuffleChanged)
    inline bool shuffle() const
    { return qvariant_cast< bool >(property("Shuffle")); }
    inline void setShuffle(bool value)
    { setProperty("Shuffle", QVariant::fromValue(value)); }

    Q_PROPERTY(double Volume READ volume WRITE setVolume NOTIFY VolumeChanged)
    inline double volume() const
    { return qvariant_cast< double >(property("Volume")); }
    inline void setVolume(double value)
    { setProperty("Volume", QVariant::fromValue(value)); }

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> Next()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Next"), argumentList);
    }

    inline QDBusPendingReply<> OpenUri(const QString &Uri)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(Uri);
        return asyncCallWithArgumentList(QStringLiteral("OpenUri"), argumentList);
    }

    inline QDBusPendingReply<> Pause()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Pause"), argumentList);
    }

    inline QDBusPendingReply<> Play()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Play"), argumentList);
    }

    inline QDBusPendingReply<> PlayPause()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("PlayPause"), argumentList);
    }

    inline QDBusPendingReply<> Previous()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Previous"), argumentList);
    }

    inline QDBusPendingReply<> Seek(qlonglong Offset)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(Offset);
        return asyncCallWithArgumentList(QStringLiteral("Seek"), argumentList);
    }

    inline QDBusPendingReply<> SetPosition(const QDBusObjectPath &TrackId, qlonglong Position)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(TrackId) << QVariant::fromValue(Position);
        return asyncCallWithArgumentList(QStringLiteral("SetPosition"), argumentList);
    }

    inline QDBusPendingReply<> Stop()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("Stop"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void Seeked(qlonglong Position);
// begin property changed signals
void CanControlChanged();
void CanGoNextChanged();
void CanGoPreviousChanged();
void CanPauseChanged();
void CanPlayChanged();
void CanSeekChanged();
void LoopStatusChanged();
void MaximumRateChanged();
void MetadataChanged();
void MinimumRateChanged();
void PlaybackStatusChanged();
void PositionChanged();
void RateChanged();
void ShuffleChanged();
void VolumeChanged();
};

namespace org {
  namespace mpris {
    namespace MediaPlayer2 {
      typedef ::DBusMediaPlayer2 Player;
    }
  }
}
#endif
