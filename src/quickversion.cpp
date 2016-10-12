#include "quickversion.h"

#include <QVersionNumber>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include "version.h"

namespace CuteVersioning {

QuickVersion::QuickVersion()
{

}

QObject *QuickVersion::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return new QuickVersion;
}

int QuickVersion::majorVersion() const
{
    return CuteVersioning::majorVersion;
}

int QuickVersion::minorVersion() const
{
    return CuteVersioning::minorVersion;
}

int QuickVersion::microVersion() const
{
    return CuteVersioning::microVersion;
}

QString QuickVersion::suffix() const
{
    return CuteVersioning::suffix;
}

QString QuickVersion::latestTag() const
{
    return CuteVersioning::latestTag;
}

QString QuickVersion::identifier() const
{
    return CuteVersioning::identifier;
}

bool QuickVersion::dirty() const
{
    return CuteVersioning::dirty;
}

}
