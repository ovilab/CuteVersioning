#ifndef CUTEVERSIONING_H
#define CUTEVERSIONING_H

#include "parser/version.h"

namespace CuteVersioning {

void init() {
    qmlRegisterSingletonType<QuickVersion>("CuteVersioning", 1, 0, "Version", &QuickVersion::qmlInstance);
}

}

#endif // CUTEVERSIONING_H
