#include "rational.h"

#include "common.h"

#include "carl/numbers/numbers.h"


void define_cln_integer(py::module& m) {
#ifdef PYCARL_USE_CLN
    py::class_<cln::cl_I>(m, "ClnInteger", "Class wrapping cln-integers")
            .def("__init__", [](cln::cl_I &instance, int val) -> void { new (&instance) cln::cl_I(val); })
            .def("__init__", [](cln::cl_I &instance, std::string val) -> void { auto tmp = carl::parse<cln::cl_I>(val); new (&instance) cln::cl_I(tmp); })

            .def(py::self + py::self)
            .def(py::self + int())
            .def(int() + py::self)

            .def(py::self - py::self)
            .def(py::self - int())
            .def(int() - py::self)

            .def(py::self * py::self)
            .def(py::self * int())
            .def(int() * py::self)

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
            .def("__repr__", [](const cln::cl_I& r) { return "<Integer (cln) " + streamToString<cln::cl_I>(r) + ">"; });

#endif
}

void define_gmp_integer(py::module& m) {
    // TODO: Avoid all the casts to mpz_class by supporting the different operator types [probably a lot of work]
    py::class_<mpz_class>(m, "GmpInteger", "Class wrapping gmp-integers")
            .def("__init__", [](mpz_class &instance, int val) -> void { new (&instance) mpz_class(val); })
            .def("__init__", [](mpz_class &instance, std::string val) -> void { auto tmp = carl::parse<mpz_class>(val); new (&instance) mpz_class(tmp); })

            .def("__add__", [](const mpz_class& lhs, const mpz_class& rhs) -> mpz_class { return lhs + rhs; })
            .def("__add__", [](const mpz_class& lhs, int rhs) -> mpz_class { return lhs + rhs; })
            .def("__add__", [](int lhs, const mpz_class& rhs) -> mpz_class { return lhs + rhs; })

            .def("__sub__", [](const mpz_class& lhs, const mpz_class& rhs) -> mpz_class { return lhs - rhs; })
            .def("__sub__", [](const mpz_class& lhs, int rhs) -> mpz_class { return lhs - rhs; })
            .def("__sub__", [](int lhs, const mpz_class& rhs) -> mpz_class { return lhs - rhs; })

            .def("__mul__", [](const mpz_class& lhs, const mpz_class& rhs) -> mpz_class { return lhs * rhs; })
            .def("__mul__", [](const mpz_class& lhs, int rhs) -> mpz_class { return lhs * rhs; })
            .def("__mul__", [](int lhs, const mpz_class& rhs) -> mpz_class { return lhs * rhs; })

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
            .def("__repr__", [](const mpz_class& r) { return "<Integer (gmp++) " + streamToString<mpz_class>(r) + ">"; });

}


