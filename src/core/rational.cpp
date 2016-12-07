/*
 * rational.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "rational.h"

#include "common.h"

#include "carl/numbers/numbers.h"

void define_rational(py::module& m) {
    py::class_<Rational>(m, "Rational", "Class wrapping rational numbers")
        .def("__init__", [](Rational &instance, double val) -> void { auto tmp = carl::rationalize<Rational>(val); new (&instance) Rational(tmp); })
        .def("__init__", [](Rational &instance, carl::sint val) -> void { auto tmp = carl::rationalize<Rational>(val); new (&instance) Rational(tmp); })
        .def("__init__", [](Rational &instance, std::string val) -> void { auto tmp = carl::rationalize<Rational>(val); new (&instance) Rational(tmp); })

        .def("__add__",  static_cast<Polynomial (*)(const Rational&, const Polynomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Rational&, const Term&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Rational&, const Monomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(const Rational&, carl::Variable::Arg)>(&carl::operator+))
        .def("__add__", [](const Rational& lhs, const Rational& rhs) -> Rational { return lhs + rhs; })
        .def("__add__", [](const Rational& lhs, double rhs) -> Rational { return lhs + carl::rationalize<Rational>(rhs); })
        .def("__add__", [](const Rational& lhs, carl::sint rhs) -> Rational { return lhs + carl::rationalize<Rational>(rhs); })
        .def("__radd__", [](const Rational& rhs, double lhs) -> Rational { return carl::rationalize<Rational>(lhs) + rhs; })
        .def("__radd__", [](const Rational& rhs, carl::sint lhs) -> Rational { return carl::rationalize<Rational>(lhs) + rhs; })

        .def("__sub__",  static_cast<Polynomial (*)(const Rational&, const Polynomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Rational&, const Term&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Rational&, const Monomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(const Rational&, carl::Variable::Arg)>(&carl::operator-))
        .def("__sub__", [](const Rational& lhs, const Rational& rhs) -> Rational { return lhs - rhs; })
        .def("__sub__", [](const Rational& lhs, double rhs) -> Rational { return lhs - carl::rationalize<Rational>(rhs); })
        .def("__sub__", [](const Rational& lhs, carl::sint rhs) -> Rational { return lhs - carl::rationalize<Rational>(rhs); })
        .def("__rsub__", [](const Rational& rhs, double lhs) -> Rational { return carl::rationalize<Rational>(lhs) - rhs; })
        .def("__rsub__", [](const Rational& rhs, carl::sint lhs) -> Rational { return carl::rationalize<Rational>(lhs) - rhs; })

        .def("__mul__",  static_cast<Polynomial (*)(const Rational&, const Polynomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(const Rational&, const Term&)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(const Rational&, const Monomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(const Rational&, carl::Variable::Arg)>(&carl::operator*))
        .def("__mul__", [](const Rational& lhs, const Rational& rhs) -> Rational { return lhs * rhs; })
        .def("__mul__", [](const Rational& lhs, double rhs) -> Rational { return lhs * carl::rationalize<Rational>(rhs); })
        .def("__mul__", [](const Rational& lhs, carl::sint rhs) -> Rational { return lhs * carl::rationalize<Rational>(rhs); })
        .def("__rmul__", [](const Rational& rhs, double lhs) -> Rational { return carl::rationalize<Rational>(lhs) * rhs; })
        .def("__rmul__", [](const Rational& rhs, carl::sint lhs) -> Rational { return carl::rationalize<Rational>(lhs) * rhs; })

        .def(PY_DIV, [](const Rational& lhs, const RationalFunction& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Rational& lhs, const Polynomial& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Rational& lhs, const Term& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Rational& lhs, const Monomial& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Rational& lhs, carl::Variable::Arg rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](const Rational& lhs, const Rational& rhs) -> Rational { if (carl::isZero(rhs)) throw std::runtime_error("Div by zero"); return lhs / rhs; })
        .def(PY_DIV, [](const Rational& lhs, double rhs) -> Rational { if (rhs == 0.0) throw std::runtime_error("Div by zero"); return lhs / carl::rationalize<Rational>(rhs); })
        .def(PY_DIV, [](const Rational& lhs, carl::sint rhs) -> Rational { if (rhs == 0.0) throw std::runtime_error("Div by zero"); return lhs / carl::rationalize<Rational>(rhs); })
        .def(PY_RDIV, [](const Rational& rhs, double lhs) -> Rational { if (carl::isZero(rhs)) throw std::runtime_error("Div by zero"); return carl::rationalize<Rational>(lhs) / rhs; })
        .def(PY_RDIV, [](const Rational& rhs, carl::sint lhs) -> Rational { if (carl::isZero(rhs)) throw std::runtime_error("Div by zero"); return carl::rationalize<Rational>(lhs) / rhs; })

        .def("__pow__", static_cast<Rational (*)(const Rational&, std::size_t)>(&carl::pow))
        .def("__pos__", [](const Rational& var) {return Rational(var);})
        .def("__neg__", [](const Rational& var) -> Rational {return -var;})
        .def("__abs__", [](const Rational& var) {return carl::abs(var);})

        .def("__eq__", [](const Rational& lhs, const Rational& rhs) -> bool { return lhs == rhs; })
        .def("__neq__", [](const Rational& lhs, const Rational& rhs) -> bool { return lhs != rhs; })
        .def("__gt__", [](const Rational& lhs, const Rational& rhs) -> bool { return lhs > rhs; })
        .def("__ge__", [](const Rational& lhs, const Rational& rhs) -> bool { return lhs >= rhs; })
        .def("__lt__", [](const Rational& lhs, const Rational& rhs) -> bool { return lhs < rhs; })
        .def("__le__", [](const Rational& lhs, const Rational& rhs) -> bool { return lhs <= rhs; })
        .def("__eq__", [](const Rational& lhs, double rhs) { return lhs == carl::rationalize<Rational>(rhs); })
        .def("__neq__", [](const Rational& lhs, double rhs) { return lhs != carl::rationalize<Rational>(rhs); })
        .def("__gt__", [](const Rational& lhs, double rhs) { return lhs > carl::rationalize<Rational>(rhs); })
        .def("__ge__", [](const Rational& lhs, double rhs) { return lhs >= carl::rationalize<Rational>(rhs); })
        .def("__lt__", [](const Rational& lhs, double rhs) { return lhs < carl::rationalize<Rational>(rhs); })
        .def("__le__", [](const Rational& lhs, double rhs) { return lhs <= carl::rationalize<Rational>(rhs); })
        .def(py::self > int())
        .def(py::self < int())
        .def(py::self == int())
        .def(py::self != int())
        .def(py::self >= int())
        .def(py::self <= int())

        .def("__int__",  [](const Rational& val) {
            double d = carl::toDouble(val);
            return static_cast<carl::sint>(d);
        })
        .def("__float__", static_cast<double (*)(Rational const&)>(&carl::toDouble))
        .def("__str__", &streamToString<Rational>)
		.def("__repr__", &streamToString<Rational>)

        .def_property_readonly("nominator", [](const Rational& val) -> int {
            return carl::toInt<carl::sint>(carl::getNum(val));
        })
        .def_property_readonly("numerator", [](const Rational& val) -> int {
            return carl::toInt<carl::sint>(carl::getNum(val));
        })
        .def_property_readonly("denominator", [](const Rational& val) -> int {
            return carl::toInt<carl::sint>(carl::getDenom(val));
        })

        .def("__getstate__", [](const Rational& val) {
            return std::pair<std::string, std::string>(carl::toString(carl::getNum(val)), carl::toString(carl::getDenom(val)));})

        .def("__setstate__", [](Rational& val, std::pair<std::string, std::string> data) {Rational res = carl::rationalize<Rational>(data.first) / carl::rationalize<Rational>(data.second); new (&val) Rational(res); })
    ;
	
	py::implicitly_convertible<double, Rational>();
	py::implicitly_convertible<carl::uint, Rational>();
}
