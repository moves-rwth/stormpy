/*
 * factorizedpolynomial.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "factorizedpolynomial.h"

#include "src/types.h"
#include "src/helpers.h"

void define_factorizationcache(py::module& m) {
    py::class_<carl::Cache<FactorizationPair>, std::shared_ptr<carl::Cache<FactorizationPair>>>(m, "FactorizationCache", "Cache storing all factorized polynomials")
        .def(py::init())
            .def("__getstate__", [](const FactorizationPair& val) -> std::tuple<std::string> { throw NoPickling(); })
            .def("__setstate__", [](FactorizationPair& val, const std::tuple<std::string>& data) { throw NoPickling(); })
    ;
}

void define_factorizedpolynomial(py::module& m) {
    py::class_<FactorizedPolynomial>(m, "FactorizedPolynomial", "Represent a polynomial with its factorization")
        .def(py::init<const Rational&>())
        .def(py::init<const Polynomial&, const std::shared_ptr<carl::Cache<FactorizationPair>>>())
        .def("is_constant", &FactorizedPolynomial::isConstant)
        .def("constant_part", &FactorizedPolynomial::constantPart)
        .def("evaluate", &FactorizedPolynomial::evaluate<Rational>)
        .def("gather_variables", static_cast<std::set<carl::Variable> (FactorizedPolynomial::*)() const>(&FactorizedPolynomial::gatherVariables))
        .def("cache", &FactorizedPolynomial::pCache)
        .def("derive", [](FactorizedPolynomial const& pol, carl::Variable const& var) {
                return pol.derivative(var, 1);
            }, "Compute the derivative", py::arg("variable"))
        .def("__str__", &streamToString<FactorizedPolynomial>)
        .def("to_smt2", [](FactorizedPolynomial const& pol) {
                return pol.toString(false, true);
            })
        .def(py::self - py::self)
        .def(py::self + py::self)
        .def(py::self * py::self)
        .def(py::self == py::self)
        .def(py::self != py::self)
            .def("__getstate__", [](const FactorizedPolynomial& val) -> std::tuple<std::string> { throw NoPickling(); })
            .def("__setstate__", [](FactorizedPolynomial& val, const std::tuple<std::string>& data) { throw NoPickling(); })
        ;
}
