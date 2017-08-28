#include "update.h"
#include "updatecontent.h"
#include <QGSettings/QGSettings>
#include <QVariant>

Update::Update(const QString &version, QWidget *parent) :
    FullscreenBackground(parent)
{
    UpdateContent *content = new UpdateContent(version, this);
    setContent(content);

    QGSettings *gsettings = new QGSettings("com.deepin.dde.appearance", "", this);
    const QStringList list = gsettings->get("background-uris").toStringList();
    setBackground(list.first());
}
