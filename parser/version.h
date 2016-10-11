#ifndef CUTE_VERSIONING_VERSION
#define CUTE_VERSIONING_VERSION

#include <QObject>
#include <QString>
#include <QQmlEngine>
#include <QVersionNumber>

namespace CuteVersioning {
class Version : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(Version)

    Q_PROPERTY(int majorVersion READ majorVersion CONSTANT)
    Q_PROPERTY(int minorVersion READ minorVersion CONSTANT)
    Q_PROPERTY(int patchVersion READ patchVersion CONSTANT)
    Q_PROPERTY(int microVersion READ microVersion CONSTANT)
    Q_PROPERTY(QString suffix READ suffix CONSTANT)
    Q_PROPERTY(QString latestTag READ latestTag CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(bool dirty READ dirty CONSTANT)

public:
    Version();

    int majorVersion() const;
    int minorVersion() const;
    int patchVersion() const;
    int microVersion() const;
    QString suffix() const;
    QString latestTag() const;
    QString description() const;
    bool dirty() const;

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
private:
    QVersionNumber m_versionNumber;
    QString m_suffix;
    QString m_description;
    QString m_latestTag;
    bool m_dirty;
};
}
#endif // CUTE_VERSIONING_VERSION
