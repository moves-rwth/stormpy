#include "common.h"

#include "gspn/gspn.h"

PYBIND11_MODULE(gspn, m) {
    m.doc() = "Support for GSPNs";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_gspn(m);
}
