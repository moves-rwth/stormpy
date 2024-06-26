#include "bound_type.h"

#include "src/types.h"
#include "src/helpers.h"


void define_boundtype(py::module& m) {
    py::enum_<carl::BoundType>(m, "BoundType")
        .value("STRICT", carl::BoundType::STRICT)
        .value("WEAK", carl::BoundType::WEAK)
        .value("INFTY", carl::BoundType::INFTY)
    ;
}
