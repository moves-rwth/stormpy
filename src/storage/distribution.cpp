#include "distribution.h"
#include "src/helpers.h"

#include "storm/storage/Distribution.h"

template<typename ValueType>
void define_distribution(py::module& m, std::string vt_suffix) {
    using Distrib = storm::storage::Distribution<ValueType, uint32_t>;

    std::string distributionClassName = std::string("Distribution") + vt_suffix;
    py::class_<Distrib> distribution(m, distributionClassName.c_str(), "Finite Support Distribution");
    distribution
            .def("__str__", &streamToString<Distrib>);
}


template void define_distribution<double>(py::module&, std::string vt_suffix);