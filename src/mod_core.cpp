#include "common.h"

#include "core/rational.h"
#include "core/variable.h"
#include "core/monomial.h"
#include "core/term.h"
#include "core/polynomial.h"
#include "core/rationalfunction.h"
#include "core/factorizedpolynomial.h"
#include "core/factorizedrationalfunction.h"

#include "core/Interval.h"

PYBIND11_PLUGIN(core) {
    py::module m("core");

    define_rational(m);
    define_variabletype(m);
    define_variable(m);
    define_monomial(m);
    define_term(m);
    define_polynomial(m);
    define_factorizationcache(m);
    define_factorizedpolynomial(m);
    define_rationalfunction(m);
    define_factorizedrationalfunction(m);
	
	define_boundtype(m);
	define_interval(m);

    // Commented out, for Python Variable can be constructed directly
    /*
    // Non-constructable class VariablePool, static instance accessible via global
    py::class_<carl::VariablePool>(m, "VariablePoolInst")
        .def("get_fresh_variable", static_cast<carl::Variable (carl::VariablePool::*)(carl::VariableType)>(&carl::VariablePool::getFreshVariable))
        .def("get_fresh_variable", static_cast<carl::Variable (carl::VariablePool::*)(const std::string&, carl::VariableType)>(&carl::VariablePool::getFreshVariable))
        .def("find_variable_with_name", &carl::VariablePool::findVariableWithName)
        ;

    // Non-constructable class MonomialPool, static instance accessible via global
    py::class_<carl::MonomialPool>(m, "MonomialPoolInst")
        .def("create", static_cast<carl::Monomial::Arg (carl::MonomialPool::*)(carl::Variable::Arg, carl::exponent)>(&carl::MonomialPool::create))
        ;

    // Setup the global variables
    m.attr("VariablePool") = py::cast(carl::VariablePool::getInstance(), py::return_value_policy::reference);
    */

    // Commented out: Use the Monomial constructors instead
    /*
    // Non-constructable class MonomialPool, static instance accessible via global
    py::class_<carl::MonomialPool>(m, "MonomialPoolInst")
        .def("create", static_cast<carl::Monomial::Arg (carl::MonomialPool::*)(carl::Variable::Arg, carl::exponent)>(&carl::MonomialPool::create))
        ;

    // Setup the global variables
    m.attr("MonomialPool") = py::cast(carl::MonomialPool::getInstance(), py::return_value_policy::reference);
    */

    return m.ptr();
}
