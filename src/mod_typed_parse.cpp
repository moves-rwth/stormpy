
#include "common.h"
#include "types.h"

#include "typed_parse/parser.h"

#include <carl-parser/Parser.h>

/**
 * The actual module definition
 */
PYBIND11_PLUGIN(parse) {
    py::module m("parse", "pycarl parsing functions");

    m.def("_deserialize", &from_string);
    m.def("_check_parsed_type", &carlparser::check_type<Polynomial>);

    return m.ptr();
}
