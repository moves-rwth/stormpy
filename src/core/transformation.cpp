#include "transformation.h"
#include "storm/models/symbolic/StandardRewardModel.h"
#include "storm/transformer/SubsystemBuilder.h"

// Thin wrappers.
template<typename VT>
storm::transformer::SubsystemBuilderReturnType<VT> constructSubsystem(storm::models::sparse::Model<VT> const& originalModel, storm::storage::BitVector const& subsystemStates, storm::storage::BitVector const& subsystemActions, bool keepUnreachableStates, storm::transformer::SubsystemBuilderOptions options) {
    return storm::transformer::buildSubsystem(originalModel, subsystemStates, subsystemActions, keepUnreachableStates, options);
}

void define_transformation(py::module& m) {
    // Transform model
    m.def("_transform_to_sparse_model", &storm::api::transformSymbolicToSparseModel<storm::dd::DdType::Sylvan, double>, "Transform symbolic model into sparse model", py::arg("model"), py::arg("formulae") = std::vector<std::shared_ptr<storm::logic::Formula const>>());
    m.def("_transform_to_sparse_parametric_model", &storm::api::transformSymbolicToSparseModel<storm::dd::DdType::Sylvan, storm::RationalFunction>, "Transform symbolic parametric model into sparse parametric model", py::arg("model"), py::arg("formulae") = std::vector<std::shared_ptr<storm::logic::Formula const>>());

    py::class_<storm::transformer::SubsystemBuilderReturnType<double>>(m, "SubsystemBuilderReturnTypeDouble", "Result of the construction of a subsystem")
            .def_readonly("model", &storm::transformer::SubsystemBuilderReturnType<double>::model, "the submodel")
            .def_readonly("new_to_old_state_mapping", &storm::transformer::SubsystemBuilderReturnType<double>::newToOldStateIndexMapping, "for each state in result, the state index in the original model")
            .def_readonly("new_to_old_action_mapping", &storm::transformer::SubsystemBuilderReturnType<double>::newToOldActionIndexMapping, "for each action in result, the action index in the original model")
            .def_readonly("kept_actions", &storm::transformer::SubsystemBuilderReturnType<double>::keptActions, "Actions of the subsystem available in the original system")
    ;

    py::class_<storm::transformer::SubsystemBuilderOptions>(m, "SubsystemBuilderOptions", "Options for constructing the subsystem")
            .def(py::init<>())
            .def_readwrite("check_transitions_outside", &storm::transformer::SubsystemBuilderOptions::checkTransitionsOutside)
            .def_readwrite("build_state_mapping", &storm::transformer::SubsystemBuilderOptions::buildStateMapping)
            .def_readwrite("build_action_mapping", &storm::transformer::SubsystemBuilderOptions::buildActionMapping)
            .def_readwrite("build_kept_actions", &storm::transformer::SubsystemBuilderOptions::buildKeptActions)
    ;


    m.def("_construct_subsystem_double", &constructSubsystem<double>, "build a subsystem of a sparse model");

}