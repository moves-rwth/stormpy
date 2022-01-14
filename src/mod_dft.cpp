#include "common.h"

#include "dft/analysis.h"
#include "dft/dft.h"
#include "dft/io.h"
#include "dft/transformations.h"

PYBIND11_MODULE(dft, m) {
    m.doc() = "Functionality for DFT analysis";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_analysis(m);
    define_analysis_typed<double>(m, "_double");
    define_analysis_typed<storm::RationalFunction>(m, "_ratfunc");
    define_dft(m);
    define_dft_typed<double>(m, "_double");
    define_dft_typed<storm::RationalFunction>(m, "_ratfunc");
    define_symmetries(m);
    define_input(m);
    define_output(m);
    define_transformations(m);
}
