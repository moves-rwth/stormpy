#include "modelchecking.h"
#include "result.h"

// Thin wrapper for model checking
double modelChecking(std::shared_ptr<storm::models::sparse::Model<double>> model, std::shared_ptr<storm::logic::Formula const> const& formula) {
    std::unique_ptr<storm::modelchecker::CheckResult> checkResult = storm::verifySparseModel<double>(model, formula);
    return checkResult->asExplicitQuantitativeCheckResult<double>()[*model->getInitialStates().begin()];
}

// Thin wrapper for model checking for all states
std::vector<double> modelCheckingAll(std::shared_ptr<storm::models::sparse::Model<double>> model, std::shared_ptr<storm::logic::Formula const> const& formula) {
    std::unique_ptr<storm::modelchecker::CheckResult> checkResult = storm::verifySparseModel<double>(model, formula);
    return checkResult->asExplicitQuantitativeCheckResult<double>().getValueVector();
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
std::pair<storm::storage::BitVector, storm::storage::BitVector> computeProb01(storm::models::sparse::Dtmc<double> model, storm::storage::BitVector const& phiStates, storm::storage::BitVector const& psiStates) {
    return storm::utility::graph::performProb01(model, phiStates, psiStates);
}

// Define python bindings
void define_modelchecking(py::module& m) {

    // Model checking
    m.def("_model_checking", &modelChecking, "Perform model checking", py::arg("model"), py::arg("formula"));
    m.def("model_checking_all", &modelCheckingAll, "Perform model checking for all states", py::arg("model"), py::arg("formula"));
    m.def("_parametric_model_checking", &parametricModelChecking, "Perform parametric model checking", py::arg("model"), py::arg("formula"));
    m.def("compute_prob01states", &computeProb01, "Compute prob-0-1 states", py::arg("model"), py::arg("phi_states"), py::arg("psi_states"));
}
