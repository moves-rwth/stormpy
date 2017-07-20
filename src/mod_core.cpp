#include "common.h"

#include "core/variable.h"
#include "core/monomial.h"
#include "core/bound_type.h"

PYBIND11_MODULE(core, m) {
    m.doc() = "pycarl core untyped functions";

    define_variabletype(m);
    define_variable(m);
    define_monomial(m);
    define_boundtype(m);

    py::register_exception<NoPickling>(m, "NoPicklingSupport");
}
