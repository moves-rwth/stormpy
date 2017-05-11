/*
 * formula.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include "formula.h"

#include "common.h"


void define_formula(py::module& m) {
    py::class_<Formula>(m, "Formula")
        .def(py::init<carl::Variable>(), "Create Formula given Boolean variable")
        .def(py::init<Constraint>())
        .def(py::init<carl::FormulaType, Formula>())
        .def(py::init<carl::FormulaType, carl::Formulas<Polynomial>>())
        .def("__init__", [](bool b) {
            return b ? Formula(carl::FormulaType::TRUE) : Formula(carl::FormulaType::FALSE);
        })
        .def("__str__", &streamToString<carl::Formula<Polynomial>>)

        .def("__not__", [](const Formula& lhs){
            return lhs.negated();
        })
        .def("__and__", [](const Formula& lhs, const Constraint& rhs){
            return Formula(carl::FormulaType::AND, {lhs, Formula(rhs)});
        })
        .def("__and__", [](const Formula& lhs, const Formula& rhs){
            return Formula(carl::FormulaType::AND, {lhs, rhs});
        })
        .def("__or__", [](const Formula& lhs, const Constraint& rhs){
            return Formula(carl::FormulaType::OR, {lhs, Formula(rhs)});
        })
        .def("__or__", [](const Formula& lhs, const Formula& rhs){
            return Formula(carl::FormulaType::OR, {lhs, rhs});
        })
        .def("__xor__", [](const Formula& lhs, const Constraint& rhs){
            return Formula(carl::FormulaType::XOR, {lhs, Formula(rhs)});
        })
        .def("__xor__", [](const Formula& lhs, const Formula& rhs){
            return Formula(carl::FormulaType::XOR, {lhs, rhs});
        })

        //.def("satisfied_by", &Formula::satisfiedBy)

        .def("__len__", &Formula::size)
        .def("__iter__", [](const Formula& f) { return py::make_iterator(f.begin(), f.end()); },
                         py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)

        .def_property_readonly("type", &Formula::getType)
        .def("__getstate__", [](const Formula& val) -> std::tuple<std::string> { throw NoPickling(); })
        .def("__setstate__", [](Formula& val, const std::tuple<std::string>& data) { throw NoPickling(); })
        ;
}
