#include "common.h"

#include "dft/dft.h"
#include "dft/dft_elements.h"
#include "dft/dft_state.h"
#include "dft/io.h"
#include "dft/analysis.h"
#include "dft/simulator.h"

PYBIND11_MODULE(dft, m) {
    m.doc() = "Functionality for DFT analysis";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_dft(m);
    define_dft_elements<double>(m, "_double");
    define_dft_elements<storm::RationalFunction>(m, "_ratfunc");
    define_dft_state<double>(m, "_double");
    define_dft_state<storm::RationalFunction>(m, "_ratfunc");
    define_failable_elements(m);
    define_symmetries(m);
    define_input(m);
    define_output(m);
    define_analysis(m);
    define_simulator(m);
    define_simulator_typed<double>(m, "_double");
    define_simulator_typed<storm::RationalFunction>(m, "_ratfunc");
}
