#include "bitvector.h"
#include "storm/storage/BitVector.h"

#include <sstream>

void define_bitvector(py::module& m) {
    using BitVector = storm::storage::BitVector;

    py::class_<BitVector>(m, "BitVector")
        .def(py::init<>())
        .def(py::init<BitVector>(), "other"_a)
        .def(py::init<uint_fast64_t>(), "length"_a)
        .def(py::init<uint_fast64_t, bool>(), "length"_a, "init"_a)
        .def(py::init<uint_fast64_t, std::vector<uint_fast64_t>>(), "length"_a, "set_entries"_a)

        .def("size", &BitVector::size)
        .def("number_of_set_bits", &BitVector::getNumberOfSetBits)
      //.def("get", &BitVector::get, "index"_a) // no idea why this does not work
        .def("get", [](BitVector const& b, uint_fast64_t i) {
                return b.get(i);
            }, "index"_a)
        .def("set", [](BitVector& b, uint_fast64_t i, bool v) {
                b.set(i, v);
            }, py::arg("index"), py::arg("value") = true, "Set")

        .def(py::self == py::self)
        .def(py::self != py::self)

        .def(py::self < py::self)
        .def(py::self & py::self)
        .def(py::self | py::self)
        .def(py::self ^ py::self)
        .def(py::self % py::self)
        .def(~py::self)

        .def(py::self &= py::self)
        .def(py::self |= py::self)

        .def("__repr__", [](BitVector const& b) { std::ostringstream oss; oss << b; return oss.str(); })

        // TODO (when needed): iterator
    ;

}
