/*
 * polynomial.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "polynomial.h"

#include "src/types.h"
#include "src/helpers.h"


void define_polynomial(py::module& m) {

    // From variable,
    // TODO clean.
    auto adder_func = static_cast<Polynomial (*)(carl::Variable::Arg, const Rational&)>(&carl::operator+);
    auto radder_func = static_cast<Polynomial (*)(const Rational&, carl::Variable::Arg)>(&carl::operator+);
    auto subs_func = static_cast<Polynomial (*)(carl::Variable::Arg, const Rational&)>(&carl::operator-);
    auto rsubs_func = static_cast<Polynomial (*)(const Rational&, carl::Variable::Arg)>(&carl::operator-);



    py::class_<Polynomial>(m, "Polynomial","Represent a multivariate polynomial")
        .def(py::init<const Term&>())
        .def(py::init<const Monomial::Arg&>())
        .def(py::init<carl::Variable::Arg>())
        .def(py::init<Rational>())

        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, const Polynomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, const Term&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, const Monomial::Arg&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, carl::Variable::Arg)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Polynomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, const Rational&)>(&carl::operator+))

        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, const Polynomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, const Term&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, const Monomial::Arg&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, carl::Variable::Arg)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, const Rational&)>(&carl::operator-))

        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, const Polynomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, const Term&)>(&carl::operator*))
        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, const Monomial::Arg&)>(&carl::operator*))
        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, carl::Variable::Arg)>(&carl::operator*))
        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, const Rational&)>(&carl::operator*))

                // From monomial TODO clean
            .def("__add__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Polynomial&)>(&carl::operator+))
            .def("__add__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Term&)>(&carl::operator+))
            .def("__add__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Monomial::Arg&)>(&carl::operator+))
            .def("__add__",  static_cast<Polynomial (*)(const Monomial::Arg&, carl::Variable::Arg)>(&carl::operator+))
            .def("__add__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Rational&)>(&carl::operator+))

            .def("__sub__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Polynomial&)>(&carl::operator-))
            .def("__sub__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Term&)>(&carl::operator-))
            .def("__sub__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Monomial::Arg&)>(&carl::operator-))
            .def("__sub__",  static_cast<Polynomial (*)(const Monomial::Arg&, carl::Variable::Arg)>(&carl::operator-))
            .def("__sub__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Rational&)>(&carl::operator-))

            .def("__mul__",  static_cast<Polynomial (*)(const Monomial::Arg&, const Polynomial&)>(&carl::operator*))


                    // From rational, TODO clean
        .def("__add__",  static_cast<Polynomial (*)(const Rational&, const Polynomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Rational&, const Term&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Rational&, const Monomial::Arg&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Rational&, carl::Variable::Arg)>(&carl::operator+))

        .def("__sub__",  static_cast<Polynomial (*)(const Rational&, const Polynomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Rational&, const Term&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Rational&, const Monomial::Arg&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Rational&, carl::Variable::Arg)>(&carl::operator-))

            .def("__mul__",  static_cast<Polynomial (*)(const Rational&, const Polynomial&)>(&carl::operator*))


                    // From variable, TODO clean
        .def("__add__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Term&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Monomial::Arg&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(carl::Variable::Arg, carl::Variable::Arg)>(&carl::operator+))
        .def("__add__",  adder_func)
        .def("__add__", [adder_func](carl::Variable::Arg lhs, carl::sint rhs) -> Polynomial {
            return adder_func(lhs, carl::rationalize<Rational>(rhs)); })
        .def("__radd__", [radder_func](carl::Variable::Arg rhs, carl::sint lhs) -> Polynomial {
            return radder_func(carl::rationalize<Rational>(lhs), rhs); })

        .def("__sub__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Polynomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Term&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Monomial::Arg&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(carl::Variable::Arg, carl::Variable::Arg)>(&carl::operator-))
        .def("__sub__",  subs_func)
        .def("__sub__", [subs_func](carl::Variable::Arg lhs, carl::sint rhs) -> Polynomial {
            return subs_func(lhs, carl::rationalize<Rational>(rhs)); })
        .def("__sub__", [rsubs_func](carl::Variable::Arg rhs, carl::sint lhs) -> Polynomial {
            return rsubs_func(carl::rationalize<Rational>(lhs), rhs); })

        .def("__mul__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Polynomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(carl::Variable, const Term&)>(&carl::operator*))
        .def("__mul__",  static_cast<Monomial::Arg (*)(carl::Variable::Arg, const Monomial::Arg&)>(&carl::operator*))
        .def("__mul__",  static_cast<Monomial::Arg (*)(carl::Variable::Arg, carl::Variable::Arg)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(carl::Variable, const Rational&)>(&carl::operator*))
        .def("__mul__", [](carl::Variable::Arg lhs, carl::sint rhs) { return lhs * carl::rationalize<Rational>(rhs); })
        .def("__rmul__", [](carl::Variable::Arg rhs, carl::sint lhs) { return carl::rationalize<Rational>(lhs) * rhs; })

        .def(PY_DIV, [](carl::Variable::Arg lhs, const RationalFunction& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, const Polynomial& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, const Term& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, const Monomial::Arg& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, carl::Variable::Arg rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable lhs, const Rational& rhs) { if (carl::isZero(rhs)) throw std::runtime_error("Div by zero"); return lhs / rhs; })
        .def(PY_DIV, [](carl::Variable lhs, carl::sint rhs) { if (rhs == 0) throw std::runtime_error("Div by zero"); return lhs / carl::rationalize<Rational>(rhs); })



        .def(PY_DIV, [](const Polynomial& lhs, const RationalFunction& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Polynomial& lhs, const Polynomial& rhs) { return RationalFunction(lhs, rhs); })
        .def(PY_DIV, [](const Polynomial& lhs, const Term& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Polynomial& lhs, const Monomial::Arg& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Polynomial& lhs, carl::Variable::Arg rhs) { return RationalFunction(lhs) / rhs; })
        .def(py::self / Rational())

        .def("__pow__", [](const Polynomial& var, carl::uint exp) {return var.pow(exp);})

        .def("__pos__", [](const Polynomial& var) {return Polynomial(var);})
        .def("__neg__", [](const Polynomial& var) {return var * Polynomial(-1);})

        .def_property_readonly("constant_part", &Polynomial::constantPart)
        .def("evaluate", &Polynomial::evaluate<Rational>)
        .def("gather_variables", static_cast<std::set<carl::Variable> (Polynomial::*)() const>(&Polynomial::gatherVariables))
        .def_property_readonly("total_degree", &Polynomial::totalDegree)//, py::doc("The maximum degree of the terms in the polynomial"))
        .def("degree", &Polynomial::degree)//, py::doc("Computes the degree with respect to the given variable"))
        .def_property_readonly("nr_terms", &Polynomial::nrTerms)
        .def("derive", [](Polynomial const& pol, carl::Variable const& var) {
                return pol.derivative(var, 1);
            }, "Compute the derivative", py::arg("variable"))
        .def("__str__", [](const Polynomial& pol) { return pol.toString(); })
        .def("to_smt2", [](Polynomial const& pol) {
                return pol.toString(false, true);
            })

        .def(py::self != py::self)
        .def(py::self == py::self)

        .def("__len__", &Polynomial::size)
        .def("__getitem__", [](const Polynomial& p, std::size_t index) { return *(p.begin()+index); })
        .def("__iter__", [](const Polynomial& p) { return py::make_iterator(p.begin(), p.end()); },
                         py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        ;
}
