#include "factorizedrationalfunction.h"


#include "src/types.h"
#include "src/helpers.h"


void define_factorizedrationalfunction(py::module& m) {
    py::class_<FactorizedRationalFunction>(m, "FactorizedRationalFunction", "Represent a rational function, that is the fraction of two factorized polynomials ")
        .def(py::init<FactorizedPolynomial, FactorizedPolynomial>())
        .def("evaluate", &FactorizedRationalFunction::evaluate)
        .def("gather_variables", static_cast<std::set<carl::Variable> (FactorizedRationalFunction::*)() const>(&FactorizedRationalFunction::gatherVariables))
        .def_property_readonly("numerator", &FactorizedRationalFunction::nominator)
        .def_property_readonly("denominator", &FactorizedRationalFunction::denominator)
        .def("derive", [](FactorizedRationalFunction const& ratFunc, carl::Variable const& var) {
                return ratFunc.derivative(var, 1);
            }, "Compute the derivative", py::arg("variable"))
        .def("__str__", &streamToString<FactorizedRationalFunction>)
        .def(py::self - py::self)
        .def(py::self + py::self)
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def(py::self == py::self)
        .def(py::self != py::self)
            .def("__getstate__", [](const FactorizedRationalFunction& val) -> std::tuple<std::string> { throw NoPickling(); })
            .def("__setstate__", [](FactorizedRationalFunction& val, const std::tuple<std::string>& data) { throw NoPickling(); })
            .def("__hash__", [](const FactorizedRationalFunction& v) { std::hash<FactorizedRationalFunction> h; return h(v);})

            ;
}
