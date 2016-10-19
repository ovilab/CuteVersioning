SYSTEM_QMAKE_BIN = $$[QT_INSTALL_BINS]/qmake
win32 {
    SYSTEM_QMAKE_BIN = $$system(where qmake)
}
unix|max {
    SYSTEM_QMAKE_BIN = $$system(which qmake)
}

!isEmpty(SYSTEM_QMAKE_BIN) {
    system(cd $$PWD/generator; qmake; make; ./generator $$GIT_DIR)
} else {
    warning(Cannot update version files: qmake not found in PATH nor bins.)
}

qmakeforce.target = dummy
qmakeforce.commands = touch $$PWD/CuteVersioning.pri # hack to force qmake to run every time
qmakeforce.depends = FORCE
POST_TARGETDEPS += $$qmakeforce.target
QMAKE_EXTRA_TARGETS += qmakeforce

#exists($$PWD/CuteVersioning/version.h) {
#    message(LOL)
    HEADERS += $$PWD/CuteVersioning/version.h
#}

HEADERS += \
    $$PWD/CuteVersioning/CuteVersioning \
    $$PWD/src/cuteversioning.h \
    $$PWD/src/init.h \
    $$PWD/src/quickversion.h \

RESOURCES +=

SOURCES += \
    $$PWD/src/quickversion.cpp

INCLUDEPATH += $$PWD
