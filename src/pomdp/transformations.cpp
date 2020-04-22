#include "transformations.h"
#include <storm-pomdp/transformer/MakePOMDPCanonic.h>

template<typename ValueType>
std::shared_ptr<storm::models::sparse::Pomdp<ValueType>> make_canonic(storm::models::sparse::Pomdp<ValueType> const& pomdp) {
    storm::transformer::MakePOMDPCanonic<ValueType> makeCanonic(pomdp);
    return makeCanonic.transform();
}

template<typename ValueType>
void define_transformations(py::module& m, std::string const& vtSuffix) {
    m.def(("_make_canonic_" + vtSuffix).c_str(), &make_canonic<ValueType>, "Return a canonicly-ordered POMDP", py::arg("pomdp"));
}

template void define_transformations<double>(py::module& m, std::string const& vtSuffix);
