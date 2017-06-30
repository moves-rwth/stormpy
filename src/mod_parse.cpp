
#include "common.h"


#include <carl-parser/Parser.h>

/**
 * The actual module definition
 */
PYBIND11_PLUGIN(parse) {
    py::module m("parse", "pycarl parsing functions");

    py::enum_<carlparser::ParserReturnType>(m, "_ParserReturnType")
            .value("Rational", carlparser::ParserReturnType::Rational)
            .value("Variable", carlparser::ParserReturnType::Variable)
            .value("Monomial", carlparser::ParserReturnType::Monomial)
            .value("Term", carlparser::ParserReturnType::Term)
            .value("Greater", carlparser::ParserReturnType::Polynomial)
            .value("RationalFunction", carlparser::ParserReturnType::RationalFunction)
            .value("Constraint", carlparser::ParserReturnType::Constraint)
            .value("Formula", carlparser::ParserReturnType::Formula)
            ;

    return m.ptr();
}
