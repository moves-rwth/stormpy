#include "modelcomponents.h"
#include "storm/models/sparse/StandardRewardModel.h"
#include "storm/models/symbolic/StandardRewardModel.h"
#include "storm/storage/sparse/ModelComponents.h"
#include "storm/storage/SparseMatrix.h"
#include "storm/models/sparse/StateLabeling.h"
#include "storm/storage/BitVector.h"




using stateLabeling = storm::models::sparse::StateLabeling;
using BitVector = storm::storage::BitVector;
template<typename ValueType> using SparseMatrix = storm::storage::SparseMatrix<ValueType>;
template<typename ValueType> using SparseRewardModel = storm::models::sparse::StandardRewardModel<ValueType>;

template<typename ValueType> using ModelComponents = storm::storage::sparse::ModelComponents<ValueType>;

// others: todo
// <storm::RationalFunction>;
//
// <storm::RationalNumber>
// <double, storm::models::sparse::StandardRewardModel<storm::Interval>>

// todo
//  1. create constructor for sparseModels double in models.h using model comp.
//  2. write tests
// 3. rationalfct

void define_model_components(py::module& m) {

    py::class_<ModelComponents<double>>(m, "ModelComponents", "ModelComponents description..")
            .def(py::init<SparseMatrix<double> const &, stateLabeling const &, std::unordered_map<std::string, SparseRewardModel<double>> const &,
                bool, boost::optional<BitVector> const &, boost::optional<SparseMatrix<storm::storage::sparse::state_type>> const &>(),
                "Construct from Prism program", py::arg("transition_matrix"), py::arg("state_labeling") = stateLabeling(),
                py::arg("reward_models") =  std::unordered_map<std::string, SparseRewardModel<double>>(), py::arg("rate_transitions") = false,
                py::arg("markovian_states") = boost::none, py::arg("player1_matrix") = boost::none)

            .def(py::init<>()) // for rvalue ? todo

            // General components (for all model types)
            .def_readwrite("transition_matrix", &ModelComponents<double>::transitionMatrix)
            .def_readwrite("state_labeling", &ModelComponents<double>::stateLabeling)
            .def_readwrite("reward_models", &ModelComponents<double>::rewardModels, "Reward models associated with the model")
            .def_readwrite("choice_labeling", &ModelComponents<double>::choiceLabeling, "A vector that stores a labeling for each choic")
            .def_readwrite("state_valuations", &ModelComponents<double>::stateValuations, "A vector that stores for each state to which variable valuation it belongs")
            .def_readwrite("choice_origins", &ModelComponents<double>::choiceOrigins, "Stores for each choice from which parts of the input model description it originates")

            // POMDP specific components
            .def_readwrite("observability_classes", &ModelComponents<double>::observabilityClasses, "The POMDP observations")

            // Continuous time specific components (CTMCs, Markov Automata):
            .def_readwrite("rate_transitions", &ModelComponents<double>::rateTransitions, "True iff the transition values (for Markovian choices) are interpreted as rates")
            .def_readwrite("exit_Rates", &ModelComponents<double>::exitRates, "The exit rate for each state. Must be given for CTMCs and MAs, if rate_transitions is false. Otherwise, it is optional.")
            .def_readwrite("markovian_states", &ModelComponents<double>::markovianStates, "A vector that stores which states are markovian (only for Markov Automata)")

            // Stochastic two player game specific components:
            .def_readwrite("player1_matrix", &ModelComponents<double>::observabilityClasses, "Matrix of player 1 choices (needed for stochastic two player games")

            ;

}