#include "rational.h"

#include "common.h"

#include "carl/numbers/numbers.h"


void define_cln_integer(py::module& m) {
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


            .def("__float__", static_cast<double (*)(Rational const&)>(&carl::toDouble))
            .def("__str__", &streamToString<cln::cl_I>)
            .def("__repr__", [](const cln::cl_I& r) { return "<Integer (cln) " + streamToString<Rational>(r) + ">"; });


}

void define_gmp_integer(py::module& m) {

}


