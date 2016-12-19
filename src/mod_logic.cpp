#include "common.h"

#include "logic/formulae.h"

PYBIND11_PLUGIN(logic) {
    py::module m("logic", "Logic module for Storm");
    
#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif
    
    define_formulae(m);
    return m.ptr();
}
