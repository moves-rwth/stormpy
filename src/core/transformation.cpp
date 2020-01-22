#include "transformation.h"
#include "storm/api/transformation.h"
#include "storm/models/symbolic/StandardRewardModel.h"
#include "storm/transformer/SubsystemBuilder.h"

// Thin wrappers.
template<typename VT>
storm::transformer::SubsystemBuilderReturnType<VT> constructSubsystem(storm::models::sparse::Model<VT> const& originalModel, storm::storage::BitVector const& subsystemStates, storm::storage::BitVector const& subsystemActions, bool keepUnreachableStates, storm::transformer::SubsystemBuilderOptions options) {
    return storm::transformer::buildSubsystem(originalModel, subsystemStates, subsystemActions, keepUnreachableStates, options);
}

template<typename ValueType>
std::pair<std::shared_ptr<storm::models::sparse::Model<ValueType>>, std::vector<std::shared_ptr<storm::logic::Formula const>>> transformContinuousToDiscreteTimeSparseModel(std::shared_ptr<storm::models::sparse::Model<ValueType>> const& model, std::vector<std::shared_ptr<storm::logic::Formula const>> const& formulas) {
    return storm::api::transformContinuousToDiscreteTimeSparseModel(model, formulas);
}

void define_transformation(py::module& m) {
    // Transform model
    m.def("_transform_to_sparse_model", &storm::api::transformSymbolicToSparseModel<storm::dd::DdType::Sylvan, double>, "Transform symbolic model into sparse model", py::arg("model"), py::arg("formulae") = std::vector<std::shared_ptr<storm::logic::Formula const>>());
    m.def("_transform_to_sparse_parametric_model", &storm::api::transformSymbolicToSparseModel<storm::dd::DdType::Sylvan, storm::RationalFunction>, "Transform symbolic parametric model into sparse parametric model", py::arg("model"), py::arg("formulae") = std::vector<std::shared_ptr<storm::logic::Formula const>>());

    m.def("_transform_to_discrete_time_model", &transformContinuousToDiscreteTimeSparseModel<double>, "Transform continuous time model to discrete time model", py::arg("model"), py::arg("formulae") = std::vector<std::shared_ptr<storm::logic::Formula const>>());
    m.def("_transform_to_discrete_time_parametric_model", &transformContinuousToDiscreteTimeSparseModel<storm::RationalFunction>, "Transform parametric continuous time model to parametric discrete time model", py::arg("model"), py::arg("formulae") = std::vector<std::shared_ptr<storm::logic::Formula const>>());

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

    // Non-Markovian chain elimination
    py::enum_<storm::transformer::EliminationLabelBehavior>(m, "EliminationLabelBehavior", "Behavior of labels while eliminating non-Markovian chains")
        .value("KEEP_LABELS", storm::transformer::EliminationLabelBehavior::KeepLabels)
        .value("MERGE_LABELS", storm::transformer::EliminationLabelBehavior::MergeLabels)
        .value("DELETE_LABELS", storm::transformer::EliminationLabelBehavior::DeleteLabels)
    ;

    m.def("_eliminate_non_markovian_chains", &storm::api::eliminateNonMarkovianChains<double>, "Eliminate chains of non-Markovian states in Markov automaton.", py::arg("ma"), py::arg("formulae"), py::arg("label_behavior"));
    m.def("_eliminate_non_markovian_chains_parametric", &storm::api::eliminateNonMarkovianChains<storm::RationalFunction>, "Eliminate chains of non-Markovian states in Markov automaton.", py::arg("ma"), py::arg("formulae"), py::arg("label_behavior"));

}