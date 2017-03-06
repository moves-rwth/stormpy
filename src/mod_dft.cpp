#include "common.h"

#include "dft/dft.h"

PYBIND11_PLUGIN(dft) {
    py::module m("dft", "Functionality for DFT analysis");

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif
    
    define_dft(m);
    return m.ptr();
}
