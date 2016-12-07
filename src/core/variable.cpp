/*
 * variable.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "variable.h"

#include "common.h"

#include "carl/core/VariablePool.h"

carl::Variable getOrCreateVariable(std::string const & name, carl::VariableType type) {
    // Variables are constructed by the Pool. Note that for a given name,
    //two Variable instances may differ, but refer to the same id (data)
    auto& pool = carl::VariablePool::getInstance();
    carl::Variable res = pool.findVariableWithName(name);
    if (res != carl::Variable::NO_VARIABLE) {
        return res;
    }
    return freshVariable(name, type);
}

void define_variabletype(py::module& m) {
    py::enum_<carl::VariableType>(m, "VariableType")
        .value("BOOL", carl::VariableType::VT_BOOL)
        .value("INT", carl::VariableType::VT_INT)
        .value("REAL", carl::VariableType::VT_REAL);
}

void define_variable(py::module& m) {
    auto adder_func = static_cast<Polynomial (*)(carl::Variable::Arg, const Rational&)>(&carl::operator+);
    auto radder_func = static_cast<Polynomial (*)(const Rational&, carl::Variable::Arg)>(&carl::operator+);
    auto subs_func = static_cast<Polynomial (*)(carl::Variable::Arg, const Rational&)>(&carl::operator-);
    auto rsubs_func = static_cast<Polynomial (*)(const Rational&, carl::Variable::Arg)>(&carl::operator-);

    py::class_<carl::Variable>(m, "Variable")
        .def("__init__",
            [](carl::Variable &instance, std::string name, carl::VariableType type) -> void {
                carl::Variable tmp = getOrCreateVariable(name, type);
                new (&instance) carl::Variable(tmp);
            }
        , py::arg("name"), py::arg("type") = carl::VariableType::VT_REAL)
        .def("__init__",
            [](carl::Variable &instance, carl::VariableType type) -> void {
                carl::Variable tmp = freshVariable(type);
                new (&instance) carl::Variable(tmp);
            }
        , py::arg("type") = carl::VariableType::VT_REAL)

        .def("__add__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Polynomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Term&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Monomial&)>(&carl::operator+))
        .def("__add__",  static_cast<Polynomial (*)(carl::Variable::Arg, carl::Variable::Arg)>(&carl::operator+))
        .def("__add__",  adder_func)
        .def("__add__", [adder_func](carl::Variable::Arg lhs, double rhs) -> Polynomial {
            return adder_func(lhs, carl::rationalize<Rational>(rhs)); })
        .def("__add__", [adder_func](carl::Variable::Arg lhs, carl::sint rhs) -> Polynomial {
            return adder_func(lhs, carl::rationalize<Rational>(rhs)); })
        .def("__radd__", [radder_func](carl::Variable::Arg rhs, double lhs) -> Polynomial  {
            return radder_func(carl::rationalize<Rational>(lhs), rhs); })
        .def("__radd__", [radder_func](carl::Variable::Arg rhs, carl::sint lhs) -> Polynomial {
            return radder_func(carl::rationalize<Rational>(lhs), rhs); })

        .def("__sub__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Polynomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Term&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Monomial&)>(&carl::operator-))
        .def("__sub__",  static_cast<Polynomial (*)(carl::Variable::Arg, carl::Variable::Arg)>(&carl::operator-))
        .def("__sub__",  subs_func)
        .def("__sub__", [subs_func](carl::Variable::Arg lhs, double rhs) -> Polynomial {
            return subs_func(lhs, carl::rationalize<Rational>(rhs)); })
        .def("__sub__", [subs_func](carl::Variable::Arg lhs, carl::sint rhs) -> Polynomial {
            return subs_func(lhs, carl::rationalize<Rational>(rhs)); })
        .def("__sub__", [rsubs_func](carl::Variable::Arg rhs, double lhs) -> Polynomial  {
            return rsubs_func(carl::rationalize<Rational>(lhs), rhs); })
        .def("__sub__", [rsubs_func](carl::Variable::Arg rhs, carl::sint lhs) -> Polynomial {
            return rsubs_func(carl::rationalize<Rational>(lhs), rhs); })

        .def("__mul__",  static_cast<Polynomial (*)(carl::Variable::Arg, const Polynomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(carl::Variable::Arg, const Term&)>(&carl::operator*))
        .def("__mul__",  static_cast<Monomial (*)(carl::Variable::Arg, const Monomial&)>(&carl::operator*))
        .def("__mul__",  static_cast<Monomial (*)(carl::Variable::Arg, carl::Variable::Arg)>(&carl::operator*))
        .def("__mul__",  static_cast<Term (*)(carl::Variable::Arg, const Rational&)>(&carl::operator*))
        .def("__mul__", [](carl::Variable::Arg lhs, double rhs) { return lhs * carl::rationalize<Rational>(rhs); })
        .def("__mul__", [](carl::Variable::Arg lhs, carl::sint rhs) { return lhs * carl::rationalize<Rational>(rhs); })
        .def("__rmul__", [](carl::Variable::Arg rhs, double lhs) { return carl::rationalize<Rational>(lhs) * rhs; })
        .def("__rmul__", [](carl::Variable::Arg rhs, carl::sint lhs) { return carl::rationalize<Rational>(lhs) * rhs; })

        .def(PY_DIV, [](carl::Variable::Arg lhs, const RationalFunction& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, const Polynomial& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, const Term& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, const Monomial& rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, carl::Variable::Arg rhs) { return RationalFunction(lhs) / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, const Rational& rhs) { if (carl::isZero(rhs)) throw std::runtime_error("Div by zero"); return lhs / rhs; })
        .def(PY_DIV, [](carl::Variable::Arg lhs, double rhs) { if (carl::isZero(rhs)) throw std::runtime_error("Div by zero"); return lhs / carl::rationalize<Rational>(rhs); })
        .def(PY_DIV, [](carl::Variable::Arg lhs, carl::sint rhs) { if (rhs == 0) throw std::runtime_error("Div by zero"); return lhs / carl::rationalize<Rational>(rhs); })

        .def("__pow__", [](carl::Variable::Arg var, carl::uint exp) -> Monomial {return carl::Monomial(var).pow(exp);})

        .def("__pos__", [](carl::Variable::Arg var) {return carl::Variable(var);})
        .def("__neg__", [](carl::Variable::Arg var) {return var * Rational(-1);})

        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self <= py::self)
        .def(py::self > py::self)
        .def(py::self >= py::self)

        .def_property_readonly("name", &carl::Variable::getName)
        .def_property_readonly("type", &carl::Variable::getType)
        .def_property_readonly("id", &carl::Variable::getId)
        .def_property_readonly("rank", &carl::Variable::getRank)
        .def("__str__", &streamToString<carl::Variable>)
        ;
}
