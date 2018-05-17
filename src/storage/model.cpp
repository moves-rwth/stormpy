#include "model.h"
#include "state.h"

#include "storm/models/ModelBase.h"
#include "storm/models/sparse/Model.h"
#include "storm/models/sparse/Dtmc.h"
#include "storm/models/sparse/Mdp.h"
#include "storm/models/sparse/Ctmc.h"
#include "storm/models/sparse/MarkovAutomaton.h"
#include "storm/models/sparse/StandardRewardModel.h"

#include <functional>
#include <string>
#include <sstream>

// Typedefs
using RationalFunction = storm::RationalFunction;
using ModelBase = storm::models::ModelBase;

template<typename ValueType> using SparseModel = storm::models::sparse::Model<ValueType>;
template<typename ValueType> using SparseDtmc = storm::models::sparse::Dtmc<ValueType>;
template<typename ValueType> using SparseMdp = storm::models::sparse::Mdp<ValueType>;
template<typename ValueType> using SparseCtmc = storm::models::sparse::Ctmc<ValueType>;
template<typename ValueType> using SparseMarkovAutomaton = storm::models::sparse::MarkovAutomaton<ValueType>;
template<typename ValueType> using SparseRewardModel = storm::models::sparse::StandardRewardModel<ValueType>;



// Thin wrappers
template<typename ValueType>
std::vector<storm::storage::sparse::state_type> getSparseInitialStates(SparseModel<ValueType> const& model) {
    std::vector<storm::storage::sparse::state_type> initialStates;
    for (auto entry : model.getInitialStates()) {
        initialStates.push_back(entry);
    }
    return initialStates;
}

template<typename ValueType>
storm::storage::SparseMatrix<ValueType>& getTransitionMatrix(SparseModel<ValueType>& model) {
    return model.getTransitionMatrix();
}

// requires pycarl.Variable
std::set<storm::RationalFunctionVariable> probabilityVariables(SparseModel<RationalFunction> const& model) {
    return storm::models::sparse::getProbabilityParameters(model);
}

std::set<storm::RationalFunctionVariable> rewardVariables(SparseModel<RationalFunction> const& model) {
    return storm::models::sparse::getRewardParameters(model);
}

template<typename ValueType>
std::function<std::string (storm::models::Model<ValueType> const&)> getModelInfoPrinter(std::string name = "Model") {
    // look, C++ has lambdas and stuff!
    return [name](storm::models::Model<ValueType> const& model) {
        std::stringstream ss;
        model.printModelInformationToStream(ss);

        // attempting a slightly readable output
        std::string text = name + " (";
        std::string line;
        for (int i = 0; std::getline(ss, line); i++) {
            if (line != "-------------------------------------------------------------- ")
                text += line + " ";
        }
        return text + ")";
    };
}

template<typename ValueType>
storm::models::sparse::StateLabeling& getLabeling(SparseModel<ValueType>& model) {
    return model.getStateLabeling();
}


// Bindings for general models
void define_model(py::module& m) {

    // ModelType
    py::enum_<storm::models::ModelType>(m, "ModelType", "Type of the model")
        .value("DTMC", storm::models::ModelType::Dtmc)
        .value("MDP", storm::models::ModelType::Mdp)
        .value("CTMC", storm::models::ModelType::Ctmc)
        .value("MA", storm::models::ModelType::MarkovAutomaton)
    ;

    // ModelBase
    py::class_<ModelBase, std::shared_ptr<ModelBase>> modelBase(m, "_ModelBase", "Base class for all models");
    modelBase.def_property_readonly("nr_states", &ModelBase::getNumberOfStates, "Number of states")
        .def_property_readonly("nr_transitions", &ModelBase::getNumberOfTransitions, "Number of transitions")
        .def_property_readonly("model_type", &ModelBase::getType, "Model type")
        .def_property_readonly("supports_parameters", &ModelBase::supportsParameters, "Flag whether model supports parameters")
        .def_property_readonly("has_parameters", &ModelBase::hasParameters, "Flag whether model has parameters")
        .def_property_readonly("is_exact", &ModelBase::isExact, "Flag whether model is exact")
        .def("_as_sparse_dtmc", [](ModelBase &modelbase) {
                return modelbase.as<SparseDtmc<double>>();
            }, "Get model as sparse DTMC")
        .def("_as_sparse_pdtmc", [](ModelBase &modelbase) {
                return modelbase.as<SparseDtmc<RationalFunction>>();
            }, "Get model as sparse pDTMC")
        .def("_as_sparse_mdp", [](ModelBase &modelbase) {
                return modelbase.as<SparseMdp<double>>();
            }, "Get model as sparse MDP")
        .def("_as_sparse_pmdp", [](ModelBase &modelbase) {
                return modelbase.as<SparseMdp<RationalFunction>>();
            }, "Get model as sparse pMDP")
        .def("_as_sparse_ctmc", [](ModelBase &modelbase) {
                return modelbase.as<SparseCtmc<double>>();
            }, "Get model as sparse CTMC")
        .def("_as_sparse_pctmc", [](ModelBase &modelbase) {
                return modelbase.as<SparseCtmc<RationalFunction>>();
            }, "Get model as sparse pCTMC")
        .def("_as_sparse_ma", [](ModelBase &modelbase) {
                return modelbase.as<SparseMarkovAutomaton<double>>();
            }, "Get model as sparse MA")
        .def("_as_sparse_pma", [](ModelBase &modelbase) {
                return modelbase.as<SparseMarkovAutomaton<RationalFunction>>();
            }, "Get model as sparse pMA")
 
    ;
}


// Bindings for sparse models
void define_sparse_model(py::module& m) {

    // Models with double numbers
    py::class_<SparseModel<double>, std::shared_ptr<SparseModel<double>>, ModelBase> model(m, "_SparseModel", "A probabilistic model where transitions are represented by doubles and saved in a sparse matrix");
    model.def_property_readonly("labeling", &getLabeling<double>, "Labels")
        .def("labels_state", &SparseModel<double>::getLabelsOfState, py::arg("state"), "Get labels of state")
        .def_property_readonly("initial_states", &getSparseInitialStates<double>, "Initial states")
        .def_property_readonly("states", [](SparseModel<double>& model) {
                return SparseModelStates<double>(model);
            }, "Get states")
        .def_property_readonly("reward_models", [](SparseModel<double>& model) {return model.getRewardModels(); }, "Reward models")
        .def_property_readonly("transition_matrix", &getTransitionMatrix<double>, py::return_value_policy::reference, py::keep_alive<1, 0>(), "Transition matrix")
        .def_property_readonly("backward_transition_matrix", &SparseModel<double>::getBackwardTransitions, py::return_value_policy::reference, py::keep_alive<1, 0>(), "Backward transition matrix")
        .def("reduce_to_state_based_rewards", &SparseModel<double>::reduceToStateBasedRewards)
        .def("__str__", getModelInfoPrinter<double>())
    ;
    py::class_<SparseDtmc<double>, std::shared_ptr<SparseDtmc<double>>>(m, "SparseDtmc", "DTMC in sparse representation", model)
        .def("__str__", getModelInfoPrinter<double>("DTMC"))
    ;
    py::class_<SparseMdp<double>, std::shared_ptr<SparseMdp<double>>>(m, "SparseMdp", "MDP in sparse representation", model)
        .def("__str__", getModelInfoPrinter<double>("MDP"))
    ;
    py::class_<SparseCtmc<double>, std::shared_ptr<SparseCtmc<double>>>(m, "SparseCtmc", "CTMC in sparse representation", model)
        .def("__str__", getModelInfoPrinter<double>("CTMC"))
    ;
    py::class_<SparseMarkovAutomaton<double>, std::shared_ptr<SparseMarkovAutomaton<double>>>(m, "SparseMA", "MA in sparse representation", model)
        .def("__str__", getModelInfoPrinter<double>("MA"))
    ;

    py::class_<SparseRewardModel<double>>(m, "SparseRewardModel", "Reward structure for sparse models")
        .def_property_readonly("has_state_rewards", &SparseRewardModel<double>::hasStateRewards)
        .def_property_readonly("has_state_action_rewards", &SparseRewardModel<double>::hasStateActionRewards)
        .def_property_readonly("has_transition_rewards", &SparseRewardModel<double>::hasTransitionRewards)
        .def_property_readonly("transition_rewards", [](SparseRewardModel<double>& rewardModel) {return rewardModel.getTransitionRewardMatrix();})
        .def_property_readonly("state_rewards", [](SparseRewardModel<double>& rewardModel) {return rewardModel.getStateRewardVector();})
        .def("get_state_reward", [](SparseRewardModel<double>& rewardModel, uint64_t state) {return rewardModel.getStateReward(state);})
        .def("get_state_action_reward", [](SparseRewardModel<double>& rewardModel, uint64_t action_index) {return rewardModel.getStateActionReward(action_index);})
        .def_property_readonly("state_action_rewards", [](SparseRewardModel<double>& rewardModel) {return rewardModel.getStateActionRewardVector();})
        .def("reduce_to_state_based_rewards", [](SparseRewardModel<double>& rewardModel, storm::storage::SparseMatrix<double> const& transitions, bool onlyStateRewards){return rewardModel.reduceToStateBasedRewards(transitions, onlyStateRewards);},  py::arg("transition_matrix"), py::arg("only_state_rewards"), "Reduce to state-based rewards")
    ;


    // Parametric models
    py::class_<SparseModel<RationalFunction>, std::shared_ptr<SparseModel<RationalFunction>>, ModelBase> modelRatFunc(m, "_SparseParametricModel", "A probabilistic model where transitions are represented by rational functions and saved in a sparse matrix");
    modelRatFunc.def("collect_probability_parameters", &probabilityVariables, "Collect parameters")
        .def("collect_reward_parameters", &rewardVariables, "Collect reward parameters")
        .def_property_readonly("labeling", &getLabeling<RationalFunction>, "Labels")
        .def("labels_state", &SparseModel<RationalFunction>::getLabelsOfState, py::arg("state"), "Get labels of state")
        .def_property_readonly("initial_states", &getSparseInitialStates<RationalFunction>, "Initial states")
        .def_property_readonly("states", [](SparseModel<RationalFunction>& model) {
                return SparseModelStates<RationalFunction>(model);
            }, "Get states")
        .def_property_readonly("reward_models", [](SparseModel<RationalFunction> const& model) {return model.getRewardModels(); }, "Reward models")
        .def_property_readonly("transition_matrix", &getTransitionMatrix<RationalFunction>, py::return_value_policy::reference, py::keep_alive<1, 0>(), "Transition matrix")
        .def_property_readonly("backward_transition_matrix", &SparseModel<RationalFunction>::getBackwardTransitions, py::return_value_policy::reference, py::keep_alive<1, 0>(), "Backward transition matrix")
        .def("reduce_to_state_based_rewards", &SparseModel<RationalFunction>::reduceToStateBasedRewards)
        .def("__str__", getModelInfoPrinter<RationalFunction>("ParametricModel"))
    ;

    py::class_<SparseDtmc<RationalFunction>, std::shared_ptr<SparseDtmc<RationalFunction>>>(m, "SparseParametricDtmc", "pDTMC in sparse representation", modelRatFunc)
        .def("__str__", getModelInfoPrinter<RationalFunction>("ParametricDTMC"))
    ;
    py::class_<SparseMdp<RationalFunction>, std::shared_ptr<SparseMdp<RationalFunction>>>(m, "SparseParametricMdp", "pMDP in sparse representation", modelRatFunc)
        .def("__str__", getModelInfoPrinter<RationalFunction>("ParametricMDP"))
    ;
    py::class_<SparseCtmc<RationalFunction>, std::shared_ptr<SparseCtmc<RationalFunction>>>(m, "SparseParametricCtmc", "pCTMC in sparse representation", modelRatFunc)
        .def("__str__", getModelInfoPrinter<RationalFunction>("ParametricCTMC"))
    ;
    py::class_<SparseMarkovAutomaton<RationalFunction>, std::shared_ptr<SparseMarkovAutomaton<RationalFunction>>>(m, "SparseParametricMA", "pMA in sparse representation", modelRatFunc)
        .def("__str__", getModelInfoPrinter<RationalFunction>("ParametricMA"))
    ;

    py::class_<SparseRewardModel<RationalFunction>>(m, "SparseParametricRewardModel", "Reward structure for parametric sparse models")
            .def_property_readonly("has_state_rewards", &SparseRewardModel<RationalFunction>::hasStateRewards)
            .def_property_readonly("has_state_action_rewards", &SparseRewardModel<RationalFunction>::hasStateActionRewards)
            .def_property_readonly("has_transition_rewards", &SparseRewardModel<RationalFunction>::hasTransitionRewards)
            .def_property_readonly("transition_rewards", [](SparseRewardModel<RationalFunction>& rewardModel) {return rewardModel.getTransitionRewardMatrix();})
            .def_property_readonly("state_rewards", [](SparseRewardModel<RationalFunction>& rewardModel) {return rewardModel.getStateRewardVector();})
            .def("get_state_reward", [](SparseRewardModel<RationalFunction>& rewardModel, uint64_t state) {return rewardModel.getStateReward(state);})
            .def("get_state_action_reward", [](SparseRewardModel<RationalFunction>& rewardModel, uint64_t action_index) {return rewardModel.getStateActionReward(action_index);})

            .def_property_readonly("state_action_rewards", [](SparseRewardModel<RationalFunction>& rewardModel) {return rewardModel.getStateActionRewardVector();})
            .def("reduce_to_state_based_rewards", [](SparseRewardModel<RationalFunction>& rewardModel, storm::storage::SparseMatrix<RationalFunction> const& transitions, bool onlyStateRewards){return rewardModel.reduceToStateBasedRewards(transitions, onlyStateRewards);},  py::arg("transition_matrix"), py::arg("only_state_rewards"), "Reduce to state-based rewards")
    ;

}

