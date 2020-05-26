#include "modelcomponents.h"
#include "storm/models/sparse/StandardRewardModel.h"
#include "storm/models/symbolic/StandardRewardModel.h"
#include "storm/storage/sparse/ModelComponents.h"


// or just template<typename ValueType> using ModelComponents = storm::storage::sparse::ModelComponents<ValueType>; todo
template<typename ValueType> using StandardRewardModel = storm::models::sparse::StandardRewardModel<ValueType>;
template<typename ValueType, typename RewardModelType = StandardRewardModel<ValueType>> using ModelComponents = storm::storage::sparse::ModelComponents<ValueType, RewardModelType>;

void define_model_components(py::module& m) {

    py::class_<ModelComponents<double, StandardRewardModel<double>>>(m, "ModelComponents", "ModelComponents description..")
            //todo .def(py::init<...>)

            // todo: readwrite or readonly?
            // general components (for all model types)
            .def_readwrite("transition_matrix", &ModelComponents<double, StandardRewardModel<double>>::transitionMatrix)
            .def_readwrite("state_labeling", &ModelComponents<double, StandardRewardModel<double>>::stateLabeling)
            .def_readwrite("reward_models", &ModelComponents<double, StandardRewardModel<double>>::rewardModels, "Reward models associated with the model")
            .def_readwrite("choice_labeling", &ModelComponents<double, StandardRewardModel<double>>::choiceLabeling, "A vector that stores a labeling for each choic")
            .def_readwrite("state_valuations", &ModelComponents<double, StandardRewardModel<double>>::stateValuations, "A vector that stores for each state to which variable valuation it belongs")
            .def_readwrite("choice_origins", &ModelComponents<double, StandardRewardModel<double>>::choiceOrigins, "Stores for each choice from which parts of the input model description it originates")

            // POMDP specific components
            .def_readwrite("observability_classes", &ModelComponents<double, StandardRewardModel<double>>::observabilityClasses, "The POMDP observations")

            // Continuous time specific components (CTMCs, Markov Automata):
            .def_readwrite("rate_transitions", &ModelComponents<double, StandardRewardModel<double>>::rateTransitions, "True iff the transition values (for Markovian choices) are interpreted as rates")
            .def_readwrite("exit_Rates", &ModelComponents<double, StandardRewardModel<double>>::exitRates, "The exit rate for each state. Must be given for CTMCs and MAs, if rate_transitions is false. Otherwise, it is optional.")
            .def_readwrite("markovian_states", &ModelComponents<double, StandardRewardModel<double>>::markovianStates, "A vector that stores which states are markovian (only for Markov Automata)")

            // Stochastic two player game specific components:
            .def_readwrite("player1_matrix", &ModelComponents<double, StandardRewardModel<double>>::observabilityClasses, "Matrix of player 1 choices (needed for stochastic two player games")


    ;
