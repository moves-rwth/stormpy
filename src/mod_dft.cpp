#include "common.h"

#include "dft/dft.h"

PYBIND11_MODULE(dft, m) {
    m.doc() = "Functionality for DFT analysis";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_dft(m);
}
