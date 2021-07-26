#include "common.h"

#include "utility/shortestPaths.h"
#include "utility/smtsolver.h"
#include "utility/chrono.h"
#include "utility/json.h"
#include "storm/adapters/RationalNumberAdapter.h"

PYBIND11_MODULE(utility, m) {
    m.doc() = "Utilities for Storm";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_ksp(m);
    define_smt(m);
    define_chrono(m);
    define_json<double>(m, "Double");
    define_json<storm::RationalNumber>(m, "Rational");
}
