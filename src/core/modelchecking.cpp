#include "modelchecking.h"
#include "result.h"

// Thin wrapper for model checking
std::shared_ptr<storm::modelchecker::CheckResult> modelChecking(std::shared_ptr<storm::models::sparse::Model<double>> model, std::shared_ptr<storm::logic::Formula const> const& formula) {
    return storm::verifySparseModel<double>(model, formula);
}

// Thin wrapper for parametric model checking
std::shared_ptr<PmcResult> parametricModelChecking(std::shared_ptr<storm::models::sparse::Model<storm::RationalFunction>> model, std::shared_ptr<storm::logic::Formula const> const& formula) {
    std::unique_ptr<storm::modelchecker::CheckResult> checkResult = storm::verifySparseModel<storm::RationalFunction>(model, formula);
    std::shared_ptr<PmcResult> result = std::make_shared<PmcResult>();
    result->resultFunction = checkResult->asExplicitQuantitativeCheckResult<storm::RationalFunction>()[*model->getInitialStates().begin()];
    storm::models::sparse::Dtmc<storm::RationalFunction>::ConstraintCollector constraintCollector(*(model->template as<storm::models::sparse::Dtmc<storm::RationalFunction>>()));
    result->constraintsWellFormed = constraintCollector.getWellformedConstraints();
    result->constraintsGraphPreserving = constraintCollector.getGraphPreservingConstraints();
    return result;
}

// Thin wrapper for computing prob01 states
template<typename ValueType>
std::pair<storm::storage::BitVector, storm::storage::BitVector> computeProb01(storm::models::sparse::Dtmc<ValueType> const& model, storm::storage::BitVector const& phiStates, storm::storage::BitVector const& psiStates) {
    return storm::utility::graph::performProb01(model, phiStates, psiStates);
}

template<typename ValueType>
std::pair<storm::storage::BitVector, storm::storage::BitVector> computeProb01min(storm::models::sparse::Mdp<ValueType> const& model, storm::storage::BitVector const& phiStates, storm::storage::BitVector const& psiStates) {
    return storm::utility::graph::performProb01Min(model, phiStates, psiStates);
}

template<typename ValueType>
std::pair<storm::storage::BitVector, storm::storage::BitVector> computeProb01max(storm::models::sparse::Mdp<ValueType> const& model, storm::storage::BitVector const& phiStates, storm::storage::BitVector const& psiStates) {
    return storm::utility::graph::performProb01Max(model, phiStates, psiStates);
}

// Define python bindings
void define_modelchecking(py::module& m) {

    // Model checking
    m.def("_model_checking", &modelChecking, "Perform model checking", py::arg("model"), py::arg("formula"));
    m.def("_parametric_model_checking", &parametricModelChecking, "Perform parametric model checking", py::arg("model"), py::arg("formula"));
    m.def("_compute_prob01states_double", &computeProb01<double>, "Compute prob-0-1 states", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
    m.def("_compute_prob01states_rationalfunc", &computeProb01<storm::RationalFunction>, "Compute prob-0-1 states", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
    m.def("_compute_prob01states_min_double", &computeProb01min<double>, "Compute prob-0-1 states (min)", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
    m.def("_compute_prob01states_max_double", &computeProb01max<double>, "Compute prob-0-1 states (max)", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
    m.def("_compute_prob01states_min_rationalfunc", &computeProb01min<storm::RationalFunction>, "Compute prob-0-1 states (min)", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
    m.def("_compute_prob01states_max_rationalfunc", &computeProb01max<storm::RationalFunction>, "Compute prob-0-1 states (max)", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
    
}
