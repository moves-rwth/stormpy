#include "common.h"

#include "parse/parser.h"

/**
 * The actual module definition
 */
PYBIND11_PLUGIN(parse) {
    py::module m("parse", "pycarl parsing functions");

    /*
    py::class_<carl::parser::Parser<Polynomial>>(m, "Parser",
    py::doc("Parser for polynomials and rational functions"))
        .def("polynomial", &carl::parser::Parser<Polynomial>::polynomial)
        .def("rational_function", &carl::parser::Parser<Polynomial>::rationalFunction)
        .def("add_variable", &carl::parser::Parser<Polynomial>::addVariable)
        ;

    py::class_<carl::parser::ExpressionParser<Polynomial>>(m, "Parser",
    py::doc("Parser for polynomials and rational functions"))
        .def("add_variable", &carl::parser::ExpressionParser<Polynomial>::addVariable)
        ;
    */

    m.def("parse", &parseString);

    //m.attr("ExpressionParser") = py::cast(ParserType(), py::return_value_policy::take_ownership);

    return m.ptr();
}
