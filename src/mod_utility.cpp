#include "common.h"

#include "utility/shortestPaths.h"

PYBIND11_PLUGIN(utility) {
    py::module m("utility", "Dumping ground of stuff that really should be somewhere more reasonable");
    define_ksp(m);
    return m.ptr();
}
