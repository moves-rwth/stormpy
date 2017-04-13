/*
 * variable.cpp
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#include <carl/core/Variable.h>
#include <carl/core/VariablePool.h>

#include "variable.h"
#include "src/helpers.h"
#include "src/types.h"


carl::Variable getOrCreateVariable(std::string const & name, carl::VariableType type) {
    // Variables are constructed by the Pool. Note that for a given name,
    // two Variable instances may differ, but refer to the same id (data)
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

    py::class_<carl::Variable>(m, "Variable")
        .def("__init__", [](carl::Variable &instance, std::string name, carl::VariableType type) {
                carl::Variable tmp = getOrCreateVariable(name, type);
                new(&instance) carl::Variable(tmp);
            }, py::arg("name"), py::arg("type") = carl::VariableType::VT_REAL)
        .def("__init__", [](carl::Variable &instance, carl::VariableType type) {
                carl::Variable tmp = freshVariable(type);
                new (&instance) carl::Variable(tmp);
            }, py::arg("type") = carl::VariableType::VT_REAL)

        .def("__mul__",  static_cast<Monomial::Arg (*)(carl::Variable::Arg, carl::Variable::Arg)>(&carl::operator*))


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
