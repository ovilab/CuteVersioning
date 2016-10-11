# Check if Python is installed
win32 {
    SYSTEM_QMAKE_BIN = $$system(where qmake)
}
unix|max {
    SYSTEM_QMAKE_BIN = $$system(which qmake)
}

!isEmpty(SYSTEM_QMAKE_BIN) {
    system(cd $$PWD/generator; qmake; make; ./generator $$GIT_DIR)
} else {
    warning(Cannot update version files: qmake not found in PATH.)
}


qmakeforce.target = dummy
qmakeforce.commands = mv Makefile Makefile.old # hack to force qmake to run every time
qmakeforce.depends = FORCE
PRE_TARGETDEPS += $$qmakeforce.target
QMAKE_EXTRA_TARGETS += qmakeforce

HEADERS += \
    $$PWD/cuteversioning.h \
    $$PWD/parser/version.h
RESOURCES += $$PWD/CuteVersioning.qrc

SOURCES += \
    $$PWD/parser/version.cpp

INCLUDEPATH += $$PWD
