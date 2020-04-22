
#include "common.h"
#include "pomdp/transformations.h"

PYBIND11_MODULE(pomdp, m) {
    m.doc() = "Functionality for POMDP analysis";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif
    define_transformations<double>(m, "Double");
}
