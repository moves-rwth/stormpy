#include "multiobjective.h"

#include "storm/modelchecker/multiobjective/multiObjectiveModelChecking.h"
#include "storm/modelchecker/multiobjective/pcaa/PcaaWeightVectorChecker.h"
#include "storm/modelchecker/multiobjective/preprocessing/SparseMultiObjectivePreprocessor.h"
#include "storm/adapters/RationalNumberAdapter.h"
#include "storm/models/sparse/Mdp.h"
#include "storm/models/sparse/StandardRewardModel.h"
#include "storm/utility/macros.h"
#include "storm/environment/Environment.h"
//
//// Helper class to avoid that we also need to bind the preprocessing.
template<typename ValueType>
std::unique_ptr<storm::modelchecker::multiobjective::PcaaWeightVectorChecker<storm::models::sparse::Mdp<ValueType>>> makeWeightedObjectiveMDPModelChecker(storm::Environment const& env, storm::models::sparse::Mdp<ValueType> const& originalModel, storm::logic::MultiObjectiveFormula const& originalFormula, bool produceScheduler) {
//    auto const preprocessresult = storm::modelchecker::multiobjective::preprocessing::SparseMultiObjectivePreprocessor<storm::models::sparse::Mdp<ValueType>>::preprocess(env, originalModel, originalFormula, produceScheduler);
//    return storm::modelchecker::multiobjective::createWeightVectorChecker(preprocessresult);
}
//
//
//// Define python bindings
template<typename ValueType>
void define_multiobjective(py::module& m, std::string const& vtSuffix) {
//    m.def(("_weighted_objective_mdp_model_checker_preprocess" + vtSuffix).c_str(), &makeWeightedObjectiveMDPModelChecker<ValueType>, py::arg("env"), py::arg("model"), py::arg("formula"), py::arg("compute_scheduler")=false);
//    m.def("_create_weighted_objective_mdp_model_checker")
//
//    using PcaaPreprocessorResult = storm::modelchecker::multiobjective::PcaaPreprocessorResult
//    using PcaaWeightVectorChecker = storm::modelchecker::multiobjective::PcaaWeightVectorChecker<storm::models::sparse::Mdp<ValueType>>;
//    py::class_<
//    py::class_<PcaaWeightVectorChecker, std::unique_ptr<PcaaWeightVectorChecker>> weightedObjectiveMdpModelChecker(m, ("WeightedObjectiveMdpModelChecker" + vtSuffix).c_str());
//    weightedObjectiveMdpModelChecker.def("check", &PcaaWeightVectorChecker::check, py::arg("env"), py::arg("weight_vector"))
//                           .def("get_achievable_point", &PcaaWeightVectorChecker::getAchievablePoint)
//                           .def("get_optimal_weighted_sum", &PcaaWeightVectorChecker::getOptimalWeightedSum, "Note that this sum can be hard to interpret whenever one mixes minimization and maximization objectives.")
//                           .def("compute_scheduler", &PcaaWeightVectorChecker::computeScheduler, "Note that this is a scheduler on the preprocessed MDP")
//                           .def("set_weighted_precision", &PcaaWeightVectorChecker::setWeightedPrecision, py::arg("value"), "A smaller value means a higher precision.");
}
//
template void define_multiobjective<double>(py::module&, std::string const&);
template void define_multiobjective<storm::RationalNumber>(py::module&, std::string const&);