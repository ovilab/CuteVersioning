#ifndef INIT_H
#define INIT_H

#include <QQmlEngine>
#include "./quickversion.h"

namespace CuteVersioning {

void init() {
    qmlRegisterSingletonType<QuickVersion>("CuteVersioning", 1, 0, "Version", &QuickVersion::qmlInstance);
}

}

#endif // INIT_H
