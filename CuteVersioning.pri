win32 {
    GIT_BIN = $$system(where git)
}
unix|max {
    GIT_BIN = $$system(which git)
}

GIT_LATEST_TAG = "git-not-found"
GIT_DESCRIPTION = "git-not-found"
GIT_DIRTY = true

!isEmpty(GIT_BIN) {
    GIT_LATEST_TAG = $$system(git --git-dir $$GIT_DIR describe --always --tags --abbrev=0)
    GIT_DESCRIPTION = $$system(git --git-dir $$GIT_DIR describe --always --tags --long)
    !system(git --git-dir $$GIT_DIR diff-index --quiet HEAD --) {
        GIT_DIRTY=false
    }
}

HEADER_OUTPUT = \
"$${LITERAL_HASH}include <QString>"\
"$${LITERAL_HASH}include <QVersionNumber>"\
""\
"namespace CuteVersioning {"\
"const QString latestTag = \"$${GIT_LATEST_TAG}\";"\
"const QString identifier = \"$${GIT_DESCRIPTION}\";"\
"const bool dirty = $${GIT_DIRTY};"\
"const QVersionNumber versionNumber = QVersionNumber::fromString(latestTag);"\
"}"\

write_file($$PWD/CuteVersioning/version.h, HEADER_OUTPUT)

qmakeforce.target = dummy
unix {
    qmakeforce.commands = touch $$PWD/CuteVersioning.pri # hack to force qmake to run every time
} win32 {
    qmakeforce.commands = type NUL >> $$PWD/CuteVersioning.pri
}
qmakeforce.depends = FORCE
POST_TARGETDEPS += $$qmakeforce.target
QMAKE_EXTRA_TARGETS += qmakeforce

exists($$PWD/CuteVersioning/version.h) {
    HEADERS += $$PWD/CuteVersioning/version.h
}

HEADERS += \
    $$PWD/CuteVersioning/CuteVersioning \
    $$PWD/src/init.h \
    $$PWD/src/quickversion.h \
    $$PWD/CuteVersioning/cuteversioning.h

SOURCES += \
    $$PWD/src/quickversion.cpp

INCLUDEPATH += $$PWD

