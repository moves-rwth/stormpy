#include "common.h"

#include "utility/shortestPaths.h"

PYBIND11_MODULE(utility, m) {
    m.doc() = "Utilities for Storm";

    define_ksp(m);
}
