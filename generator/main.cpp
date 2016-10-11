#include <QProcess>
#include <QDebug>
#include <QJsonDocument>
#include <QFile>
#include <QJsonObject>

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
    QFile file("version_info.json");
    file.open(QFile::WriteOnly);
    file.write(doc.toJson());

    return 0;
}
