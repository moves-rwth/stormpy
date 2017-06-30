#include "common.h"

#include "core/variable.h"
#include "core/monomial.h"

#include "core/BoundType.h"

PYBIND11_PLUGIN(core) {
    py::module m("core");


    define_variabletype(m);
    define_variable(m);
    define_monomial(m);
    define_boundtype(m);

    py::register_exception<NoPickling>(m, "NoPicklingSupport");


    return m.ptr();
}
