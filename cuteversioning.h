#ifndef CUTEVERSIONING_H
#define CUTEVERSIONING_H

#include "parser/version.h"

namespace CuteVersioning {

void init() {
    qmlRegisterSingletonType<Version>("CuteVersioning", 1, 0, "Version", &Version::qmlInstance);
}

}

#endif // CUTEVERSIONING_H
