#include "factorizedpolynomial.h"

#include "src/types.h"
#include "src/helpers.h"

void define_factorizationcache(py::module& m) {
    py::class_<carl::Cache<FactorizationPair>, std::shared_ptr<carl::Cache<FactorizationPair>>>(m, "FactorizationCache", "Cache storing all factorized polynomials")
        .def(py::init())
        .def("__getstate__", [](const FactorizationPair& val) -> std::tuple<std::string> { throw NoPickling(); })
        .def("__setstate__", [](FactorizationPair& val, const std::tuple<std::string>& data) { throw NoPickling(); })
    ;
}

void define_factorization(py::module& m) {
    py::class_<Factorization, std::shared_ptr<Factorization>>(m, "Factorization", "Factorization")
        .def("__str__", &streamToString<Factorization>)
        .def(py::self == py::self)

        .def("__len__", [](const Factorization& f) {
                return f.size();
            })
        .def("__iter__", [](const Factorization& f) {
                return py::make_iterator(f.begin(), f.end());
            }, py::keep_alive<0, 1>() /* Essential: keep object alive while iterator exists */)

        .def("__getstate__", [](const Factorization& val) -> std::tuple<std::string> { throw NoPickling(); })
        .def("__setstate__", [](Factorization& val, const std::tuple<std::string>& data) { throw NoPickling(); })
    ;
}

void define_factorizedpolynomial(py::module& m) {
    py::class_<FactorizedPolynomial>(m, "FactorizedPolynomial", "Represent a polynomial with its factorization")
        .def(py::init<const Rational&>())
        .def(py::init<const Polynomial&, const std::shared_ptr<carl::Cache<FactorizationPair>>>())
        .def("is_constant", &FactorizedPolynomial::isConstant)
        .def("constant_part", &FactorizedPolynomial::constantPart)
        .def("coefficient", &FactorizedPolynomial::coefficient)

        .def("factorization", &FactorizedPolynomial::factorization, "Get factorization")
        .def("polynomial", &FactorizedPolynomial::polynomial, "Get underlying polynomial")

        .def("evaluate", &FactorizedPolynomial::evaluate<Rational>)
        .def("gather_variables", static_cast<std::set<carl::Variable> (FactorizedPolynomial::*)() const>(&FactorizedPolynomial::gatherVariables))
        .def("cache", &FactorizedPolynomial::pCache)
        .def("derive", [](FactorizedPolynomial const& pol, carl::Variable const& var) {
                return pol.derivative(var, 1);
            }, "Compute the derivative", py::arg("variable"))
        .def("__str__", &streamToString<FactorizedPolynomial>)
        .def("to_smt2", [](FactorizedPolynomial const& pol) {
                return pol.toString(false, true);
            })
        .def(py::self - py::self)
        .def(py::self + py::self)
        .def(py::self * py::self)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self == Rational())
        .def(py::self != Rational())
        .def("__getstate__", [](const FactorizedPolynomial& val) -> std::tuple<std::string> { throw NoPickling(); })
        .def("__setstate__", [](FactorizedPolynomial& val, const std::tuple<std::string>& data) { throw NoPickling(); })
        .def("__hash__", [](const FactorizedPolynomial& v) { std::hash<FactorizedPolynomial> h; return h(v);})
    ;
}
