#include "common.h"

#include "pars/pars.h"
#include "pars/pla.h"
#include "pars/model_instantiator.h"

PYBIND11_PLUGIN(pars) {
    py::module m("pars", "Functionality for parametric analysis");

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    define_pars(m);
    define_pla(m);
    define_model_instantiator(m);
    return m.ptr();
}
