#include <QProcess>
#include <QDebug>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>
#include <QVersionNumber>

QString runCommand(QString command, QStringList arguments) {
    QProcess process;
    process.start(command, arguments);
    if(!process.waitForFinished()) {
        qFatal("Process call never returned.");
    }
    return process.readAll().replace("\n", "");
}

int main(int argc, char** argv) {
    if(argc < 2) {
        qFatal("Too few arguments.\nUsage: parser <.git dir>");
    }

    QString gitDir(argv[1]);

    QString latestTag = runCommand("git", {"--git-dir", gitDir, "describe", "--always", "--tags", "--abbrev=0"});
    QString description = runCommand("git", {"--git-dir", gitDir, "describe", "--always", "--tags", "--long"});
    QProcess dirtyProcess;
    dirtyProcess.start("git", {"diff-index", "--quiet", "HEAD", "--"});
    if(!dirtyProcess.waitForFinished()) {
        qFatal("QProcess call never returned");
    }
    bool dirty = (dirtyProcess.exitCode() > 0);

    QJsonObject root;
    root["latestTag"] = latestTag;
    root["description"] = description;
    root["dirty"] = dirty;
    QJsonDocument doc(root);
    QFile jsonFile("version_info.json");
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());

    int suffixIndex = 0;
    QVersionNumber versionNumber = QVersionNumber::fromString(latestTag, &suffixIndex);
    QString suffix;
    if(suffixIndex > 0) {
        suffix = latestTag.mid(suffixIndex);
    }

    QFile file("version_info.h");
    file.open(QFile::WriteOnly);

    QString contents = "#include <QString>\n"
                       "\n"
                       "namespace CuteVersioning {\n"
                       "const QString latestTag = \"%1\";\n"
                       "const QString description = \"%2\";\n"
                       "const bool dirty = %3;\n"
                       "const int majorVersion = %4;\n"
                       "const int minorVersion = %5;\n"
                       "const int microVersion = %6;\n"
                       "const QString suffix = \"%7\";\n"
                       "}\n";

    contents = contents.arg(latestTag,
                            description,
                            dirty ? "true" : "false",
                            QString::number(versionNumber.majorVersion()),
                            QString::number(versionNumber.minorVersion()),
                            QString::number(versionNumber.microVersion()),
                            suffix
                            );

    file.write(contents.toLatin1());

    return 0;
}
