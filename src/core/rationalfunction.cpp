/*
 * rationalfunction.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "rationalfunction.h"


#include "src/types.h"
#include "src/helpers.h"



void define_rationalfunction(py::module& m) {
    py::class_<RationalFunction>(m, "RationalFunction", "Represent a rational function, that is the fraction of two multivariate polynomials ")
        .def(py::init<Polynomial, Polynomial>())

        .def("__add__",  static_cast<RationalFunction (*)(const RationalFunction&, const Polynomial&)>(&carl::operator+))
        .def("__add__",  static_cast<RationalFunction (*)(const RationalFunction&, const Term&)>(&carl::operator+))
        .def("__add__",  static_cast<RationalFunction (*)(const RationalFunction&, const Monomial::Arg&)>(&carl::operator+))
        .def("__add__",  static_cast<RationalFunction (*)(const RationalFunction&, carl::Variable::Arg)>(&carl::operator+))
        .def("__add__",  static_cast<RationalFunction (*)(const RationalFunction&, const Rational&)>(&carl::operator+))

        .def("__sub__",  static_cast<RationalFunction (*)(const RationalFunction&, const Polynomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<RationalFunction (*)(const RationalFunction&, const Term&)>(&carl::operator-))
        .def("__sub__",  static_cast<RationalFunction (*)(const RationalFunction&, const Monomial::Arg&)>(&carl::operator-))
        .def("__sub__",  static_cast<RationalFunction (*)(const RationalFunction&, carl::Variable::Arg)>(&carl::operator-))
        .def("__sub__",  static_cast<RationalFunction (*)(const RationalFunction&, const Rational&)>(&carl::operator-))

        .def("__mul__",  static_cast<RationalFunction (*)(const RationalFunction&, const Polynomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<RationalFunction (*)(const RationalFunction&, const Term&)>(&carl::operator*))
        .def("__mul__",  static_cast<RationalFunction (*)(const RationalFunction&, const Monomial::Arg&)>(&carl::operator*))
        .def("__mul__",  static_cast<RationalFunction (*)(const RationalFunction&, carl::Variable::Arg)>(&carl::operator*))
        .def("__mul__",  static_cast<RationalFunction (*)(const RationalFunction&, const Rational&)>(&carl::operator*))

        .def(PY_DIV, static_cast<RationalFunction (*)(const RationalFunction&, const Rational&)>(&carl::operator/))
        .def(PY_DIV, static_cast<RationalFunction (*)(const RationalFunction&, const Polynomial&)>(&carl::operator/))
        .def(PY_DIV, static_cast<RationalFunction (*)(const RationalFunction&, const Term&)>(&carl::operator/))
        .def(PY_DIV, static_cast<RationalFunction (*)(const RationalFunction&, carl::Variable::Arg)>(&carl::operator/))
        .def(PY_DIV, static_cast<RationalFunction (*)(const RationalFunction&, const Rational&)>(&carl::operator/))

        // From monomial, todo clean
            .def(PY_DIV, [](const Monomial::Arg& lhs, const RationalFunction& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
            .def(PY_DIV, [](const Monomial::Arg& lhs, const Polynomial& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
            .def(PY_DIV, [](const Monomial::Arg& lhs, const Term& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
            .def(PY_DIV, [](const Monomial::Arg& lhs, const Monomial::Arg& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
            .def(PY_DIV, [](const Monomial::Arg& lhs, const carl::Variable& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })


                    // From rational, todo clean.
            .def(PY_DIV, [](const Rational& lhs, const RationalFunction& rhs) { return RationalFunction(lhs) / rhs; })
            .def(PY_DIV, [](const Rational& lhs, const Polynomial& rhs) { return RationalFunction(lhs) / rhs; })
            .def(PY_DIV, [](const Rational& lhs, const Term& rhs) { return RationalFunction(lhs) / rhs; })
            .def(PY_DIV, [](const Rational& lhs, const Monomial::Arg& rhs) { return RationalFunction(lhs) / rhs; })
            .def(PY_DIV, [](const Rational& lhs, carl::Variable::Arg rhs) { return RationalFunction(lhs) / rhs; })


            .def("__pow__", [](const RationalFunction& var, carl::uint exp) {return carl::pow(var, exp);})

        .def("__pos__", [](const RationalFunction& var) {return RationalFunction(var);})
        .def("__neg__", [](const RationalFunction& var) {return var * RationalFunction(-1);})

        .def("evaluate", &RationalFunction::evaluate)
        .def("gather_variables", static_cast<std::set<carl::Variable> (RationalFunction::*)() const>(&RationalFunction::gatherVariables))
        .def_property_readonly("nominator", &RationalFunction::nominator)
        .def_property_readonly("numerator", &RationalFunction::nominator)
        .def_property_readonly("denominator", &RationalFunction::denominator)
        .def("derive", [](RationalFunction const& ratFunc, carl::Variable const& var) {
                return ratFunc.derivative(var, 1);
            }, "Compute the derivative", py::arg("variable"))
        .def("__str__", &streamToString<RationalFunction>)

        .def(py::self == py::self)
        .def(py::self != py::self)
        ;
}
