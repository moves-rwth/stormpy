/*
 * polynomial.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "polynomial.h"

#include "common.h"

void define_polynomial(py::module& m) {
    py::class_<Polynomial>(m, "Polynomial","Represent a multivariate polynomial")
        .def(py::init<const Term&>())
        .def(py::init<const Monomial&>())
        .def(py::init<carl::Variable::Arg>())
        .def(py::init<Rational>())

        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, const Polynomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, const Term&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, const Monomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, carl::Variable::Arg)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Polynomial&, const Rational&)>(&carl::operator+))

        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, const Polynomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, const Term&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, const Monomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, carl::Variable::Arg)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Polynomial&, const Rational&)>(&carl::operator-))

        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, const Polynomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, const Term&)>(&carl::operator*))
        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, const Monomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, carl::Variable::Arg)>(&carl::operator*))
        .def("__mul__",  static_cast<Polynomial (*)(const Polynomial&, const Rational&)>(&carl::operator*))

        .def(PY_DIV, [](const Polynomial& lhs, const RationalFunction& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Polynomial& lhs, const Polynomial& rhs) { return RationalFunction(lhs, rhs); })
        .def(PY_DIV, [](const Polynomial& lhs, const Term& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Polynomial& lhs, const Monomial& rhs) { return RationalFunction(lhs) / rhs; })
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
        .def("__str__", [](const Polynomial& var) { return var.toString(); })

        .def(py::self != py::self)
        .def(py::self == py::self)

       .def("__len__", &Polynomial::size)
       .def("__getitem__", [](const Polynomial& p, std::size_t index) { return *(p.begin()+index); })
       .def("__iter__", [](const Polynomial& p) { return py::make_iterator(p.begin(), p.end()); },
                        py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        ;
}
