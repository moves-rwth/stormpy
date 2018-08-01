#include "common.h"

#include "dft/dft.h"
#include "dft/io.h"
#include "dft/analysis.h"

PYBIND11_MODULE(dft, m) {
    m.doc() = "Functionality for DFT analysis";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_dft(m);
    define_input(m);
    define_output(m);
    define_analysis(m);
}
