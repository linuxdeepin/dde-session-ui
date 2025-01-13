/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp ./dde-session-ui/xml/org.deepin.dde.SoundEffect1.xml -a ./dde-session-ui/toolGenerate/qdbusxml2cpp/org.deepin.dde.SoundEffect1Adaptor -i ./dde-session-ui/toolGenerate/qdbusxml2cpp/org.deepin.dde.SoundEffect1.h
 *
 * qdbusxml2cpp is Copyright (C) 2017 The Qt Company Ltd.
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef ORG_DEEPIN_DDE_SOUNDEFFECT1ADAPTOR_H
#define ORG_DEEPIN_DDE_SOUNDEFFECT1ADAPTOR_H

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "./dde-session-ui/toolGenerate/qdbusxml2cpp/org.deepin.dde.SoundEffect1.h"
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface org.deepin.dde.SoundEffect1
 */
class SoundEffect1Adaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dde.SoundEffect1")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.deepin.dde.SoundEffect1\">\n"
"    <method name=\"EnableSound\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
"      <arg direction=\"in\" type=\"b\" name=\"enabled\"/>\n"
"    </method>\n"
"    <method name=\"GetSoundEnabledMap\">\n"
"      <arg direction=\"out\" type=\"a{sb}\" name=\"result\"/>\n"
"      <annotation value=\"QMap&lt;QString,bool&gt;\" name=\"org.qtproject.QtDBus.QtTypeName.Out0\"/>\n"
"    </method>\n"
"    <method name=\"GetSoundFile\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"file\"/>\n"
"    </method>\n"
"    <method name=\"GetSystemSoundFile\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
"      <arg direction=\"out\" type=\"s\" name=\"file\"/>\n"
"    </method>\n"
"    <method name=\"IsSoundEnabled\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
"      <arg direction=\"out\" type=\"b\" name=\"enabled\"/>\n"
"    </method>\n"
"    <method name=\"PlaySound\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
"    </method>\n"
"    <method name=\"PlaySystemSound\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"name\"/>\n"
"    </method>\n"
"    <property access=\"readwrite\" type=\"b\" name=\"Enabled\"/>\n"
"  </interface>\n"
        "")
public:
    SoundEffect1Adaptor(QObject *parent);
    virtual ~SoundEffect1Adaptor();

public: // PROPERTIES
    Q_PROPERTY(bool Enabled READ enabled WRITE setEnabled)
    bool enabled() const;
    void setEnabled(bool value);

public Q_SLOTS: // METHODS
    void EnableSound(const QString &name, bool enabled);
    QMap<QString,bool> GetSoundEnabledMap();
    QString GetSoundFile(const QString &name);
    QString GetSystemSoundFile(const QString &name);
    bool IsSoundEnabled(const QString &name);
    void PlaySound(const QString &name);
    void PlaySystemSound(const QString &name);
Q_SIGNALS: // SIGNALS
};

#endif
