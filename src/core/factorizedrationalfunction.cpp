/*
 * factorizedraitonalfunction.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "factorizedrationalfunction.h"

#include "common.h"

void define_factorizedrationalfunction(py::module& m) {
    py::class_<FactorizedRationalFunction>(m, "FactorizedRationalFunction", "Represent a rational function, that is the fraction of two factorized polynomials ")
        .def(py::init<FactorizedPolynomial, FactorizedPolynomial>())
        .def("evaluate", &FactorizedRationalFunction::evaluate)
        .def("gather_variables", static_cast<std::set<carl::Variable> (FactorizedRationalFunction::*)() const>(&FactorizedRationalFunction::gatherVariables))
        .def_property_readonly("numerator", &FactorizedRationalFunction::nominator)
        .def_property_readonly("denominator", &FactorizedRationalFunction::denominator)
        .def("__str__", &streamToString<FactorizedRationalFunction>)
        .def(py::self - py::self)
        .def(py::self + py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self == py::self)
        .def(py::self != py::self)
        ;
}
