/*
 * monomial.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "monomial.h"

#include "common.h"

void define_monomial(py::module& m) {
    py::class_<Monomial, std::shared_ptr<Monomial>>(m, "Monomial")
//        .def("__init__", [] (Monomial& m, const carl::Variable& v, carl::exponent e){
//            m = carl::createMonomial(v, e);
//        })

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
    .def("__mul__",  static_cast<Term (*)(const Monomial::Arg&, const Term&)>(&carl::operator*))
    .def("__mul__",  static_cast<Monomial::Arg (*)(const Monomial::Arg&, const Monomial::Arg&)>(&carl::operator*))
    .def("__mul__",  static_cast<Monomial::Arg (*)(const Monomial::Arg&, carl::Variable::Arg)>(&carl::operator*))
    .def("__mul__",  static_cast<Term (*)(const Monomial::Arg&, const Rational&)>(&carl::operator*))

    .def(PY_DIV, [](const Monomial::Arg& lhs, const RationalFunction& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
    .def(PY_DIV, [](const Monomial::Arg& lhs, const Polynomial& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
    .def(PY_DIV, [](const Monomial::Arg& lhs, const Term& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
    .def(PY_DIV, [](const Monomial::Arg& lhs, const Monomial::Arg& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
    .def(PY_DIV, [](const Monomial::Arg& lhs, const carl::Variable& rhs) { return RationalFunction(Polynomial(lhs)) / rhs; })
    
    .def(PY_DIV, [](const Monomial::Arg& lhs, Rational rhs) { return lhs / rhs; })

    .def("__pow__", [](const Monomial::Arg& var, carl::uint exp) {return var->pow(exp);})

    .def("__pos__", [](const Monomial::Arg& var) {return Monomial::Arg(var);})
    .def("__neg__", [](const Monomial::Arg& var) {return var * Rational(-1);})

    .def_property_readonly("tdeg", [](const Monomial::Arg& arg) { return arg->tdeg();} )
    .def_property_readonly("exponents", [](const Monomial::Arg& arg) { return arg->exponents();} )
    .def("__str__", &streamToString<const Monomial::Arg&>)

    .def("__len__", [](const Monomial::Arg& m) { return m->nrVariables(); })
    .def("__getitem__", [](const Monomial::Arg& m, std::size_t index) { return *(m->begin()+index); })
    .def("__iter__", [](const Monomial::Arg& m) { return py::make_iterator(m->begin(), m->end()); },
                        py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)
        ;
    
    
    m.def("create_monomial", [] (const carl::Variable& v, carl::exponent e){
        return carl::createMonomial(v, e);
    });

}
