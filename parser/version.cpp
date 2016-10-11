#include "version.h"

#include <QVersionNumber>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

CuteVersioning::Version::Version() {
    QFile file(":/CuteVersioning/generator/version_info.json");
    if(!file.open(QFile::ReadOnly)) {
        qWarning() << "Could not open version_info.json.";
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject root = doc.object();
    m_latestTag = root["latestTag"].toString();
    m_description = root["description"].toString();
    m_dirty = root["dirty"].toBool();

    int suffixIndex = 0;
    m_versionNumber = QVersionNumber::fromString(m_latestTag, &suffixIndex);
    m_suffix = m_latestTag.mid(suffixIndex);
}

QObject *CuteVersioning::Version::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return new Version;
}

int CuteVersioning::Version::majorVersion() const
{
    return m_versionNumber.majorVersion();
}

int CuteVersioning::Version::minorVersion() const
{
    return m_versionNumber.minorVersion();
}

int CuteVersioning::Version::patchVersion() const
{
    return m_versionNumber.microVersion();
}

int CuteVersioning::Version::microVersion() const
{
    return m_versionNumber.microVersion();
}

QString CuteVersioning::Version::suffix() const
{
    return m_suffix;
}

QString CuteVersioning::Version::latestTag() const
{
    return m_latestTag;
}

QString CuteVersioning::Version::description() const
{
    return m_description;
}

bool CuteVersioning::Version::dirty() const
{
    return m_dirty;
}
