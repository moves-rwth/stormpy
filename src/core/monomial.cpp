/*
 * monomial.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "monomial.h"

#include "common.h"

void define_monomial(py::module& m) {
    py::class_<Monomial>(m, "Monomial")
        .def("__init__", [] (Monomial& arg, carl::Variable v, carl::exponent e) -> void {
            auto m = carl::MonomialPool::getInstance().create(v, e);
            arg = m;
        })

        .def("__add__",  static_cast<Polynomial (*)(const Monomial&, const Polynomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Monomial&, const Term&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Monomial&, const Monomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Monomial&, carl::Variable::Arg)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Monomial&, const Rational&)>(&carl::operator+))

        .def("__sub__",  static_cast<Polynomial (*)(const Monomial&, const Polynomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Monomial&, const Term&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Monomial&, const Monomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Monomial&, carl::Variable::Arg)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Monomial&, const Rational&)>(&carl::operator-))

        .def("__mul__",  static_cast<Polynomial (*)(const Monomial&, const Polynomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(const Monomial&, const Term&)>(&carl::operator*))
        .def("__mul__",  static_cast<Monomial (*)(const Monomial&, const Monomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Monomial (*)(const Monomial&, carl::Variable::Arg)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(const Monomial&, const Rational&)>(&carl::operator*))

        .def(PY_DIV, [](const Monomial& lhs, const RationalFunction& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
        .def(PY_DIV, [](const Monomial& lhs, const Polynomial& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
        .def(PY_DIV, [](const Monomial& lhs, const Term& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
        .def(PY_DIV, [](const Monomial& lhs, const Monomial& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
        .def(PY_DIV, [](const Monomial& lhs, carl::Variable::Arg rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
        .def(py::self / Rational())

        .def("__pow__", [](const Monomial& var, carl::uint exp) {return var->pow(exp);})

        .def("__pos__", [](const Monomial& var) {return Monomial(var);})
        .def("__neg__", [](const Monomial& var) {return var * Rational(-1);})

        .def_property_readonly("tdeg", [](carl::Monomial::Arg& arg) { return arg->tdeg();} )
        .def_property_readonly("exponents", [](carl::Monomial::Arg& arg) { return arg->exponents();} )
        .def("__str__", &streamToString<carl::Monomial::Arg>)

       .def("__len__", [](const Monomial& m) { return m->nrVariables(); })
       .def("__getitem__", [](const Monomial& m, std::size_t index) { return *(m->begin()+index); })
       .def("__iter__", [](const Monomial& m) { return py::make_iterator(m->begin(), m->end()); },
                        py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        ;
}
