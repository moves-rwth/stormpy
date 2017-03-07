#include "model.h"
#include "storm/models/ModelBase.h"
#include "storm/models/sparse/Model.h"
#include "storm/models/sparse/Dtmc.h"
#include "storm/models/sparse/Mdp.h"
#include "storm/models/sparse/StandardRewardModel.h"
#include "storm/utility/ModelInstantiator.h"

// Thin wrapper for getting initial states
template<typename ValueType>
std::vector<storm::storage::sparse::state_type> getInitialStates(storm::models::sparse::Model<ValueType> const& model) {
    std::vector<storm::storage::sparse::state_type> initialStates;
    for (auto entry : model.getInitialStates()) {
        initialStates.push_back(entry);
    }
    return initialStates;
}

// Thin wrapper for getting transition matrix
template<typename ValueType>
storm::storage::SparseMatrix<ValueType>& getTransitionMatrix(storm::models::sparse::Model<ValueType>& model) {
    return model.getTransitionMatrix();
}

std::set<storm::RationalFunctionVariable> probabilityVariables(storm::models::sparse::Model<storm::RationalFunction> const& model) {
    return storm::models::sparse::getProbabilityParameters(model);
}

std::set<storm::RationalFunctionVariable> rewardVariables(storm::models::sparse::Model<storm::RationalFunction> const& model) {
    return storm::models::sparse::getRewardParameters(model);
}

template<typename ValueType>
storm::models::sparse::StateLabeling& getLabeling(storm::models::sparse::Model<ValueType>& model) {
    return model.getStateLabeling();
}

// Define python bindings
void define_model(py::module& m) {

    // ModelType
    py::enum_<storm::models::ModelType>(m, "ModelType", "Type of the model")
        .value("DTMC", storm::models::ModelType::Dtmc)
        .value("MDP", storm::models::ModelType::Mdp)
        .value("CTMC", storm::models::ModelType::Ctmc)
        .value("MA", storm::models::ModelType::MarkovAutomaton)
    ;

    // ModelBase
    py::class_<storm::models::ModelBase, std::shared_ptr<storm::models::ModelBase>> modelBase(m, "_ModelBase", "Base class for all models");
    modelBase.def_property_readonly("nr_states", &storm::models::ModelBase::getNumberOfStates, "Number of states")
        .def_property_readonly("nr_transitions", &storm::models::ModelBase::getNumberOfTransitions, "Number of transitions")
        .def_property_readonly("model_type", &storm::models::ModelBase::getType, "Model type")
        .def_property_readonly("supports_parameters", &storm::models::ModelBase::supportsParameters, "Flag whether model supports parameters")
        .def_property_readonly("has_parameters", &storm::models::ModelBase::hasParameters, "Flag whether model has parameters")
        .def_property_readonly("is_exact", &storm::models::ModelBase::isExact, "Flag whether model is exact")
        .def("_as_dtmc", &storm::models::ModelBase::as<storm::models::sparse::Dtmc<double>>, "Get model as DTMC")
        .def("_as_pdtmc", &storm::models::ModelBase::as<storm::models::sparse::Dtmc<storm::RationalFunction>>, "Get model as pDTMC")
        .def("_as_mdp", &storm::models::ModelBase::as<storm::models::sparse::Mdp<double>>, "Get model as MDP")
        .def("_as_pmdp", &storm::models::ModelBase::as<storm::models::sparse::Mdp<storm::RationalFunction>>, "Get model as pMDP")
        .def("_as_ctmc", &storm::models::ModelBase::as<storm::models::sparse::Ctmc<double>>, "Get model as CTMC")
        .def("_as_pctmc", &storm::models::ModelBase::as<storm::models::sparse::Ctmc<storm::RationalFunction>>, "Get model as pCTMC")
        .def("_as_ma", &storm::models::ModelBase::as<storm::models::sparse::MarkovAutomaton<double>>, "Get model as MA")
        .def("_as_pma", &storm::models::ModelBase::as<storm::models::sparse::MarkovAutomaton<storm::RationalFunction>>, "Get model as pMA")
    ;

    // Models
//storm::models::sparse::Model<double, storm::models::sparse::StandardRewardModel<double> >

    py::class_<storm::models::sparse::Model<double>, std::shared_ptr<storm::models::sparse::Model<double>>> model(m, "_SparseModel", "A probabilistic model where transitions are represented by doubles and saved in a sparse matrix", modelBase);
    model.def_property_readonly("labeling", &getLabeling<double>, "Labels")
        .def("labels_state", &storm::models::sparse::Model<double>::getLabelsOfState, py::arg("state"), "Get labels of state")
        .def_property_readonly("initial_states", &getInitialStates<double>, "Initial states")
        .def_property_readonly("transition_matrix", &getTransitionMatrix<double>, py::return_value_policy::reference, py::keep_alive<1, 0>(), "Transition matrix")
    ;
    py::class_<storm::models::sparse::Dtmc<double>, std::shared_ptr<storm::models::sparse::Dtmc<double>>>(m, "SparseDtmc", "DTMC in sparse representation", model)
    ;
    py::class_<storm::models::sparse::Mdp<double>, std::shared_ptr<storm::models::sparse::Mdp<double>>>(m, "SparseMdp", "MDP in sparse representation", model)
    ;
    py::class_<storm::models::sparse::Ctmc<double>, std::shared_ptr<storm::models::sparse::Ctmc<double>>>(m, "SparseCtmc", "CTMC in sparse representation", model)
    ;
    py::class_<storm::models::sparse::MarkovAutomaton<double>, std::shared_ptr<storm::models::sparse::MarkovAutomaton<double>>>(m, "SparseMA", "MA in sparse representation", model)
    ;

    py::class_<storm::models::sparse::Model<storm::RationalFunction>, std::shared_ptr<storm::models::sparse::Model<storm::RationalFunction>>> modelRatFunc(m, "_SparseParametricModel", "A probabilistic model where transitions are represented by rational functions and saved in a sparse matrix", modelBase);
    modelRatFunc.def("collect_probability_parameters", &probabilityVariables, "Collect parameters")
            .def("collect_reward_parameters", &rewardVariables, "Collect reward parameters")
        .def_property_readonly("labeling", &getLabeling<storm::RationalFunction>, "Labels")
        .def("labels_state", &storm::models::sparse::Model<storm::RationalFunction>::getLabelsOfState, py::arg("state"), "Get labels of state")
        .def_property_readonly("initial_states", &getInitialStates<storm::RationalFunction>, "Initial states")
        .def_property_readonly("transition_matrix", &getTransitionMatrix<storm::RationalFunction>, py::return_value_policy::reference, py::keep_alive<1, 0>(), "Transition matrix")
    ;
    py::class_<storm::models::sparse::Dtmc<storm::RationalFunction>, std::shared_ptr<storm::models::sparse::Dtmc<storm::RationalFunction>>>(m, "SparseParametricDtmc", "pDTMC in sparse representation", modelRatFunc)
    ;
    py::class_<storm::models::sparse::Mdp<storm::RationalFunction>, std::shared_ptr<storm::models::sparse::Mdp<storm::RationalFunction>>>(m, "SparseParametricMdp", "pMDP in sparse representation", modelRatFunc)
    ;
    py::class_<storm::models::sparse::Ctmc<storm::RationalFunction>, std::shared_ptr<storm::models::sparse::Ctmc<storm::RationalFunction>>>(m, "SparseParametricCtmc", "pCTMC in sparse representation", modelRatFunc)
    ;
    py::class_<storm::models::sparse::MarkovAutomaton<storm::RationalFunction>, std::shared_ptr<storm::models::sparse::MarkovAutomaton<storm::RationalFunction>>>(m, "SparseParametricMA", "pMA in sparse representation", modelRatFunc)
    ;

}

// Model instantiator
void define_model_instantiator(py::module& m) {
    py::class_<storm::utility::ModelInstantiator<storm::models::sparse::Dtmc<storm::RationalFunction>,storm::models::sparse::Dtmc<double>>>(m, "PdtmcInstantiator", "Instantiate PDTMCs to DTMCs")
        .def(py::init<storm::models::sparse::Dtmc<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<storm::models::sparse::Dtmc<storm::RationalFunction>, storm::models::sparse::Dtmc<double>>::instantiate, "Instantiate model with given parameter values");

    py::class_<storm::utility::ModelInstantiator<storm::models::sparse::Mdp<storm::RationalFunction>,storm::models::sparse::Mdp<double>>>(m, "PmdpInstantiator", "Instantiate PMDPs to MDPs")
        .def(py::init<storm::models::sparse::Mdp<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<storm::models::sparse::Mdp<storm::RationalFunction>, storm::models::sparse::Mdp<double>>::instantiate, "Instantiate model with given parameter values");
    
    py::class_<storm::utility::ModelInstantiator<storm::models::sparse::Ctmc<storm::RationalFunction>,storm::models::sparse::Ctmc<double>>>(m, "PctmcInstantiator", "Instantiate PCTMCs to CTMCs")
        .def(py::init<storm::models::sparse::Ctmc<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<storm::models::sparse::Ctmc<storm::RationalFunction>, storm::models::sparse::Ctmc<double>>::instantiate, "Instantiate model with given parameter values");
    
    py::class_<storm::utility::ModelInstantiator<storm::models::sparse::MarkovAutomaton<storm::RationalFunction>,storm::models::sparse::MarkovAutomaton<double>>>(m, "PmaInstantiator", "Instantiate PMAs to MAs")
        .def(py::init<storm::models::sparse::MarkovAutomaton<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<storm::models::sparse::MarkovAutomaton<storm::RationalFunction>, storm::models::sparse::MarkovAutomaton<double>>::instantiate, "Instantiate model with given parameter values");
}
