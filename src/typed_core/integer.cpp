#include "rational.h"

#include "src/helpers.h"

#include "src/types.h"
#include "carl/numbers/numbers.h"
#include "carl/numbers/conversion/cln_gmp.h"


void define_cln_integer(py::module& m) {
#ifdef PYCARL_USE_CLN
    py::class_<cln::cl_I>(m, "Integer", "Class wrapping cln-integers")
        .def("__init__", [](cln::cl_I &instance, int val) -> void { new (&instance) cln::cl_I(val); })
        .def("__init__", [](cln::cl_I &instance, std::string val) -> void {
            cln::cl_I tmp;
            bool suc = carl::try_parse<cln::cl_I>(val, tmp);
            if(!suc) {
                throw std::invalid_argument("Cannot translate " + val + " into an integer.");
            }
             new (&instance) cln::cl_I(tmp); })
        .def("__init__", [](cln::cl_I &instance, mpz_class const& val) -> void { auto tmp = carl::convert<mpz_class, cln::cl_I>(val); new (&instance) cln::cl_I(tmp);})

        .def(py::self + py::self)
        .def(py::self + int())
        .def(int() + py::self)
        .def("__add__", [](const cln::cl_I& lhs, carl::Variable::Arg rhs) -> Polynomial { return cln::cl_RA(lhs) + rhs; })
        .def("__add__", [](const cln::cl_I& lhs, const Monomial::Arg& rhs) -> Polynomial { return cln::cl_RA(lhs) + rhs; })
        .def("__radd__", [](const cln::cl_I& rhs, carl::Variable::Arg lhs) -> Polynomial { return cln::cl_RA(rhs) + lhs; })
        .def("__radd__", [](const cln::cl_I& rhs, const Monomial::Arg& lhs) -> Polynomial { return cln::cl_RA(rhs) + lhs; })

        .def(py::self - py::self)
        .def(py::self - int())
        .def(int() - py::self)
        .def("__sub__", [](const cln::cl_I& lhs, carl::Variable::Arg rhs) -> Polynomial { return cln::cl_RA(lhs) - rhs; })
        .def("__sub__", [](const cln::cl_I& lhs, const Monomial::Arg& rhs) -> Polynomial { return cln::cl_RA(lhs) - rhs; })

        .def(py::self * py::self)
        .def(py::self * int())
        .def(int() * py::self)
        .def("__mul__", [](const cln::cl_I& lhs, carl::Variable::Arg rhs) -> Term { return cln::cl_RA(lhs) * rhs; })
        .def("__mul__", [](const cln::cl_I& lhs, const Monomial::Arg& rhs) -> Term { return cln::cl_RA(lhs) * rhs; })
        .def("__rmul__", [](const cln::cl_I& rhs, carl::Variable::Arg lhs) -> Term { return cln::cl_RA(rhs) * lhs; })
        .def("__rmul__", [](const cln::cl_I& rhs, const Monomial::Arg& lhs) -> Term { return cln::cl_RA(rhs) * lhs; })

        .def(PY_DIV, [](const cln::cl_I& lhs, const cln::cl_I& rhs) -> cln::cl_RA { if (carl::isZero(rhs)) throw std::runtime_error("Div by zero"); return lhs / rhs; })

        .def("__pow__", static_cast<cln::cl_I (*)(const cln::cl_I&, std::size_t)>(&carl::pow))
        .def("__pos__", [](const cln::cl_I& var) {return var;})
        .def("__neg__", [](const cln::cl_I& var) -> cln::cl_I {return -var;})
        .def("__abs__", [](const cln::cl_I& var) {return carl::abs(var);})

        .def(py::self > py::self)
        .def(py::self < py::self)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self >= py::self)
        .def(py::self <= py::self)

        .def(py::self > int())
        .def(py::self < int())
        .def(py::self == int())
        .def(py::self != int())
        .def(py::self >= int())
        .def(py::self <= int())


        .def("__float__", static_cast<double (*)(cln::cl_I const&)>(&carl::toDouble))
        .def("__str__", &streamToString<cln::cl_I>)
        .def("__repr__", [](const cln::cl_I& r) { return "<Integer (cln) " + streamToString<cln::cl_I>(r) + ">"; })
        .def("__getstate__", [](const cln::cl_I& val) {
            return std::tuple<std::string>(carl::toString(val));})

        .def("__setstate__", [](cln::cl_I& val, std::tuple<std::string> data) {cln::cl_I res = carl::parse<cln::cl_I>(std::get<0>(data)); new (&val) cln::cl_I(res); })
        .def("__hash__", [](const cln::cl_I& v) { std::hash<cln::cl_I> h; return h(v);})
    ;
#endif
}

void define_gmp_integer(py::module& m) {
#ifndef PYCARL_USE_CLN
    py::class_<mpz_class>(m, "Integer", "Class wrapping gmp-integers")
        .def("__init__", [](mpz_class &instance, int val) -> void { new (&instance) mpz_class(val); })
        .def("__init__", [](mpz_class &instance, std::string const& val)  -> void {
            mpz_class tmp;
            bool suc = carl::try_parse<mpz_class>(val, tmp);
            if(!suc) {
                throw std::invalid_argument("Cannot translate " + val + " into an integer.");
            }
            new (&instance) mpz_class(tmp); })
#ifdef PYCARL_HAS_CLN
        .def("__init__", [](mpz_class &instance, cln::cl_I const& val) -> void { auto tmp = carl::convert<cln::cl_I, mpz_class>(val); new (&instance) mpz_class(tmp);})
#endif

        .def("__add__", [](const mpz_class& lhs, const mpz_class& rhs) -> mpz_class { return lhs + rhs; })
        .def("__add__", [](const mpz_class& lhs, int rhs) -> mpz_class { return lhs + rhs; })
        .def("__radd__", [](const mpz_class& lhs, int rhs) -> mpz_class { return lhs + rhs; })
        .def("__add__", [](const mpz_class& lhs, carl::Variable::Arg rhs) -> Polynomial { return mpq_class(lhs) + rhs; })
        .def("__add__", [](const mpz_class& lhs, const Monomial::Arg& rhs) -> Polynomial { return mpq_class(lhs) + rhs; })
        .def("__radd__", [](const mpz_class& lhs, carl::Variable::Arg rhs) -> Polynomial { return mpq_class(lhs) + rhs; })
        .def("__radd__", [](const mpz_class& lhs, const Monomial::Arg& rhs) -> Polynomial { return mpq_class(lhs) + rhs; })

        .def("__sub__", [](const mpz_class& lhs, const mpz_class& rhs) -> mpz_class { return lhs - rhs; })
        .def("__sub__", [](const mpz_class& lhs, int rhs) -> mpz_class { return lhs - rhs; })
        .def("__sub__", [](int lhs, const mpz_class& rhs) -> mpz_class { return lhs - rhs; })
        .def("__sub__", [](const mpz_class& lhs, const Monomial::Arg& rhs) -> Polynomial { return mpq_class(lhs) - rhs; })
        .def("__sub__", [](const mpz_class& lhs, carl::Variable::Arg rhs) -> Polynomial { return mpq_class(lhs) - rhs; })
        .def("__rsub__", [](const mpz_class& lhs, const Monomial::Arg& rhs) -> Polynomial { return rhs - mpq_class(lhs); })
        .def("__rsub__", [](const mpz_class& lhs, carl::Variable::Arg rhs) -> Polynomial { return rhs - mpq_class(lhs); })

        .def("__mul__", [](const mpz_class& lhs, const mpz_class& rhs) -> mpz_class { return lhs * rhs; })
        .def("__mul__", [](const mpz_class& lhs, int rhs) -> mpz_class { return lhs * rhs; })
        .def("__mul__", [](int lhs, const mpz_class& rhs) -> mpz_class { return lhs * rhs; })

        .def("__mul__", [](const mpz_class& lhs, carl::Variable::Arg rhs) -> Term { return mpq_class(lhs) * rhs; })
        .def("__mul__", [](const mpz_class& lhs, const Monomial::Arg& rhs) -> Term { return mpq_class(lhs) * rhs; })
        .def("__rmul__", [](const mpz_class& rhs, carl::Variable::Arg lhs) -> Term { return mpq_class(rhs) * lhs; })
        .def("__rmul__", [](const mpz_class& rhs, const Monomial::Arg& lhs) -> Term { return mpq_class(rhs) * lhs; })

        .def(PY_DIV, [](const mpz_class& lhs, const mpz_class& rhs) -> mpq_class { if (carl::isZero(rhs)) throw std::runtime_error("Div by zero"); return lhs / rhs; })

        .def("__pow__", static_cast<mpz_class (*)(const mpz_class&, std::size_t)>(&carl::pow))
        .def("__pos__", [](const mpz_class& var) {return var;})
        .def("__neg__", [](const mpz_class& var) -> mpz_class {return -var;})
        .def("__abs__", [](const mpz_class& var) {return carl::abs(var);})

        .def(py::self > py::self)
        .def(py::self < py::self)
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self >= py::self)
        .def(py::self <= py::self)

        .def(py::self > int())
        .def(py::self < int())
        .def(py::self == int())
        .def(py::self != int())
        .def(py::self >= int())
        .def(py::self <= int())


        .def("__float__", static_cast<double (*)(mpz_class const&)>(&carl::toDouble))
        .def("__str__", &streamToString<mpz_class>)
        .def("__repr__", [](const mpz_class& r) { return "<Integer (gmp) " + streamToString<mpz_class>(r) + ">"; })
        .def("__getstate__", [](const mpz_class& val) {
            return std::tuple<std::string>(carl::toString(val));})
        .def("__setstate__", [](mpz_class& val, std::tuple<std::string> data) {mpz_class res = carl::parse<mpz_class>(std::get<0>(data)); new (&val) mpz_class(res); })
        .def("__hash__", [](const mpz_class& v) { std::hash<mpz_class> h; return h(v);})
    ;
#endif

}


