#include "transformation.h"
#include "storm/models/symbolic/StandardRewardModel.h"

void define_transformation(py::module& m) {
    // Transform model
    m.def("_transform_to_sparse_model", &storm::api::transformSymbolicToSparseModel<storm::dd::DdType::Sylvan, double>, "Transform symbolic model into sparse model", py::arg("model"), py::arg("formulae"));
    m.def("_transform_to_sparse_parametric_model", &storm::api::transformSymbolicToSparseModel<storm::dd::DdType::Sylvan, storm::RationalFunction>, "Transform symbolic parametric model into sparse parametric model", py::arg("model"), py::arg("formulae"));
}