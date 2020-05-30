#include "modelcomponents.h"

#include "storm/models/sparse/StandardRewardModel.h"
#include "storm/models/symbolic/StandardRewardModel.h"
#include "storm/storage/sparse/ModelComponents.h"
#include "storm/storage/SparseMatrix.h"
#include "storm/models/sparse/StateLabeling.h"
#include "storm/storage/BitVector.h"


using StateLabeling = storm::models::sparse::StateLabeling;
using BitVector = storm::storage::BitVector;
template<typename ValueType> using SparseMatrix = storm::storage::SparseMatrix<ValueType>;
template<typename ValueType> using SparseRewardModel = storm::models::sparse::StandardRewardModel<ValueType>;

template<typename ValueType> using SparseModelComponents = storm::storage::sparse::ModelComponents<ValueType>;

// others: todo
// <storm::RationalFunction>;
//
// <storm::RationalNumber>
// <double, storm::models::sparse::StandardRewardModel<storm::Interval>>

// Parametric models, Valuetype: <storm::RationalFunction> todo

void define_sparse_modelcomponents(py::module& m) {

    py::class_<SparseModelComponents<double>>(m, "SparseModelComponents", "ModelComponents description..") //todo

        .def(py::init<SparseMatrix<double> const&, StateLabeling const&, std::unordered_map<std::string, SparseRewardModel<double>> const&,
            bool, boost::optional<BitVector> const&, boost::optional<SparseMatrix<storm::storage::sparse::state_type>> const&>(),
            py::arg("transition_matrix"), py::arg("state_labeling") = storm::models::sparse::StateLabeling(),
            py::arg("reward_models") =  std::unordered_map<std::string, SparseRewardModel<double>>(), py::arg("rate_transitions") = false,
            py::arg("markovian_states") = boost::none, py::arg("player1_matrix") = boost::none)

        //.def(py::init<>()) // for rvalue ? todo

        // General components (for all model types)
        .def_readwrite("transition_matrix", &SparseModelComponents<double>::transitionMatrix)
        .def_readwrite("state_labeling", &SparseModelComponents<double>::stateLabeling)
        .def_readwrite("reward_models", &SparseModelComponents<double>::rewardModels, "Reward models associated with the model")
        .def_readwrite("choice_labeling", &SparseModelComponents<double>::choiceLabeling, "A vector that stores a labeling for each choice")
        .def_readwrite("state_valuations", &SparseModelComponents<double>::stateValuations, "A vector that stores for each state to which variable valuation it belongs")
        .def_readwrite("choice_origins", &SparseModelComponents<double>::choiceOrigins, "Stores for each choice from which parts of the input model description it originates")

        // POMDP specific components
        .def_readwrite("observability_classes", &SparseModelComponents<double>::observabilityClasses, "The POMDP observations")

        // Continuous time specific components (CTMCs, Markov Automata):
        .def_readwrite("rate_transitions", &SparseModelComponents<double>::rateTransitions, "True iff the transition values (for Markovian choices) are interpreted as rates")
        .def_readwrite("exit_Rates", &SparseModelComponents<double>::exitRates, "The exit rate for each state. Must be given for CTMCs and MAs, if rate_transitions is false. Otherwise, it is optional.")
        .def_readwrite("markovian_states", &SparseModelComponents<double>::markovianStates, "A vector that stores which states are markovian (only for Markov Automata)")

        // Stochastic two player game specific components:
        .def_readwrite("player1_matrix", &SparseModelComponents<double>::observabilityClasses, "Matrix of player 1 choices (needed for stochastic two player games")
        ;

}