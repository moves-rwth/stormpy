#include "model.h"
#include "storm/models/ModelBase.h"
#include "storm/models/sparse/Model.h"
#include "storm/models/sparse/Dtmc.h"
#include "storm/models/sparse/Mdp.h"
#include "storm/models/sparse/StandardRewardModel.h"
#include "storm/utility/ModelInstantiator.h"

#include <functional>
#include <string>
#include <sstream>


using ModelBase = storm::models::ModelBase;
using state_type = storm::storage::sparse::state_type;
using RationalFunction = storm::RationalFunction;
using RationalFunctionVariable = storm::RationalFunctionVariable;
template<typename ValueType> using Model = storm::models::sparse::Model<ValueType>;
template<typename ValueType> using Dtmc = storm::models::sparse::Dtmc<ValueType>;
template<typename ValueType> using Mdp = storm::models::sparse::Mdp<ValueType>;
template<typename ValueType> using SparseMatrix = storm::storage::SparseMatrix<ValueType>;

// Thin wrapper for getting initial states
template<typename ValueType>
std::vector<state_type> getInitialStates(Model<ValueType> const& model) {
    std::vector<state_type> initialStates;
    for (auto entry : model.getInitialStates()) {
        initialStates.push_back(entry);
    }
    return initialStates;
}

// Thin wrapper for getting transition matrix
template<typename ValueType>
SparseMatrix<ValueType>& getTransitionMatrix(Model<ValueType>& model) {
    return model.getTransitionMatrix();
}

// requires pycarl.Variable
std::set<RationalFunctionVariable> probabilityVariables(Model<RationalFunction> const& model) {
    return storm::models::sparse::getProbabilityParameters(model);
}

std::set<RationalFunctionVariable> rewardVariables(Model<RationalFunction> const& model) {
    return storm::models::sparse::getRewardParameters(model);
}

template<typename ValueType>
std::function<std::string (Model<ValueType> const&)> getModelInfoPrinter(std::string name = "Model") {
    // look, C++ has lambdas and stuff!
    return [name](Model<ValueType> const& model) {
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
    py::class_<ModelBase, std::shared_ptr<ModelBase>> modelBase(m, "_ModelBase", "Base class for all models");
    modelBase.def_property_readonly("nr_states", &ModelBase::getNumberOfStates, "Number of states")
        .def_property_readonly("nr_transitions", &ModelBase::getNumberOfTransitions, "Number of transitions")
        .def_property_readonly("model_type", &ModelBase::getType, "Model type")
        .def_property_readonly("supports_parameters", &ModelBase::supportsParameters, "Flag whether model supports parameters")
        .def_property_readonly("has_parameters", &ModelBase::hasParameters, "Flag whether model has parameters")
        .def_property_readonly("is_exact", &ModelBase::isExact, "Flag whether model is exact")
        .def("_as_dtmc", &ModelBase::as<Dtmc<double>>, "Get model as DTMC")
        .def("_as_pdtmc", &ModelBase::as<Dtmc<RationalFunction>>, "Get model as pDTMC")
        .def("_as_mdp", &ModelBase::as<Mdp<double>>, "Get model as MDP")
        .def("_as_pmdp", &ModelBase::as<Mdp<RationalFunction>>, "Get model as pMDP")
    ;

    // Models
//storm::models::sparse::Model<double, storm::models::sparse::StandardRewardModel<double> >

    py::class_<Model<double>, std::shared_ptr<Model<double>>> model(m, "_SparseModel", "A probabilistic model where transitions are represented by doubles and saved in a sparse matrix", modelBase);
    model.def_property_readonly("labels", [](Model<double>& model) {
            return model.getStateLabeling().getLabels();
        }, "Labels")
        .def("labels_state", &Model<double>::getLabelsOfState, py::arg("state"), "Get labels of state")
        .def_property_readonly("initial_states", &getInitialStates<double>, "Initial states")
        .def_property_readonly("transition_matrix", &getTransitionMatrix<double>, py::return_value_policy::reference, py::keep_alive<1, 0>(), "Transition matrix")
        .def("__str__", getModelInfoPrinter<double>())
    ;
    py::class_<Dtmc<double>, std::shared_ptr<Dtmc<double>>>(m, "SparseDtmc", "DTMC in sparse representation", model)
        .def("__str__", getModelInfoPrinter<double>("DTMC"))
    ;
    py::class_<Mdp<double>, std::shared_ptr<Mdp<double>>>(m, "SparseMdp", "MDP in sparse representation", model)
        .def("__str__", getModelInfoPrinter<double>("MDP"))
    ;

    py::class_<Model<RationalFunction>, std::shared_ptr<Model<RationalFunction>>> modelRatFunc(m, "_SparseParametricModel", "A probabilistic model where transitions are represented by rational functions and saved in a sparse matrix", modelBase);
    modelRatFunc.def("collect_probability_parameters", &probabilityVariables, "Collect parameters")
        .def("collect_reward_parameters", &rewardVariables, "Collect reward parameters")
        .def_property_readonly("labels", [](Model<RationalFunction>& model) {
                return model.getStateLabeling().getLabels();
            }, "Labels")
        .def("labels_state", &Model<RationalFunction>::getLabelsOfState, py::arg("state"), "Get labels of state")
        .def_property_readonly("initial_states", &getInitialStates<RationalFunction>, "Initial states")
        .def_property_readonly("transition_matrix", &getTransitionMatrix<RationalFunction>, py::return_value_policy::reference, py::keep_alive<1, 0>(), "Transition matrix")
        .def("__str__", getModelInfoPrinter<RationalFunction>("ParametricModel"))
    ;
    py::class_<Dtmc<RationalFunction>, std::shared_ptr<Dtmc<RationalFunction>>>(m, "SparseParametricDtmc", "pDTMC in sparse representation", modelRatFunc)
        .def("__str__", getModelInfoPrinter<RationalFunction>("ParametricDTMC"))
    ;
    py::class_<Mdp<RationalFunction>, std::shared_ptr<Mdp<RationalFunction>>>(m, "SparseParametricMdp", "pMDP in sparse representation", modelRatFunc)
        .def("__str__", getModelInfoPrinter<RationalFunction>("ParametricMDP"))
    ;

}

// Model instantiator
void define_model_instantiator(py::module& m) {
    py::class_<storm::utility::ModelInstantiator<Dtmc<RationalFunction>, Dtmc<double>>>(m, "PdtmcInstantiator", "Instantiate PDTMCs to DTMCs")
        .def(py::init<Dtmc<RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<Dtmc<RationalFunction>, Dtmc<double>>::instantiate, "Instantiate model with given parameter values")
    ;

    py::class_<storm::utility::ModelInstantiator<Mdp<RationalFunction>,Mdp<double>>>(m, "PmdpInstantiator", "Instantiate PMDPs to MDPs")
        .def(py::init<Mdp<RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<Mdp<RationalFunction>, Mdp<double>>::instantiate, "Instantiate model with given parameter values")
    ;
}
