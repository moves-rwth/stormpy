#include "multiobjective.h"

#include "storm/modelchecker/multiobjective/multiObjectiveModelChecking.h"

#include "storm/environment/Environment.h"
#include "storm/environment/modelchecker/MultiObjectiveModelCheckerEnvironment.h"
#include "storm/modelchecker/multiobjective/constraintbased/SparseCbAchievabilityQuery.h"
#include "storm/modelchecker/multiobjective/deterministicScheds/DeterministicSchedsAchievabilityChecker.h"
#include "storm/modelchecker/multiobjective/deterministicScheds/DeterministicSchedsParetoExplorer.h"
#include "storm/modelchecker/multiobjective/pcaa/SparsePcaaAchievabilityQuery.h"
#include "storm/modelchecker/multiobjective/pcaa/SparsePcaaParetoQuery.h"
#include "storm/modelchecker/multiobjective/pcaa/SparsePcaaQuantitativeQuery.h"
#include "storm/modelchecker/multiobjective/pcaa/StandardMdpPcaaWeightVectorChecker.h"
#include "storm/modelchecker/multiobjective/preprocessing/SparseMultiObjectivePreprocessor.h"
#include "storm/models/sparse/MarkovAutomaton.h"
#include "storm/models/sparse/Mdp.h"
#include "storm/models/sparse/StandardRewardModel.h"
#include "storm/settings/SettingsManager.h"
#include "storm/settings/modules/CoreSettings.h"
#include "storm/utility/Stopwatch.h"
#include "storm/utility/macros.h"

#include "storm/exceptions/InvalidArgumentException.h"
#include "storm/exceptions/InvalidEnvironmentException.h"


std::pair<double, double> _checkForRelReach(storm::Environment const& env, storm::models::sparse::Mdp<double> const& model,
                                                                storm::logic::MultiObjectiveFormula const& formula) {
    // This internal check for RelReach requires some cleanup before it makes sense to merge this.
    //    STORM_LOG_ASSERT(model.getInitialStates().getNumberOfSetBits() == 1,
    //                     "Multi-objective Model checking on model with multiple initial states is not supported.");
    std::vector<double> weightVector = {1.0,-1.0};
    // Preprocess the model
    auto preprocessorResult = storm::modelchecker::multiobjective::preprocessing::SparseMultiObjectivePreprocessor<storm::models::sparse::Mdp<double>>::preprocess(env, model, formula);
    auto checker = storm::modelchecker::multiobjective::StandardMdpPcaaWeightVectorChecker(preprocessorResult);
    checker.check(env, weightVector);
    double underApprox = checker.getUnderApproximationOfInitialStateResults().at(0);
    double overApprox = checker.getOverApproximationOfInitialStateResults().at(0);
//
//    /*!
//     * Retrieves a scheduler that induces the current values
//     * Note that check(..) has to be called before retrieving the scheduler. Otherwise, an exception is thrown.
//     * Also note that (currently) the scheduler only supports unbounded objectives.
//     */
//    virtual storm::storage::Scheduler<ValueType> computeScheduler() const override;
    return {underApprox, overApprox};
}


// Define python bindings
void define_multiobjective(py::module& m) {
    m.def("compute_rel_reach_helper", &_checkForRelReach, py::arg("env"), py::arg("model"), py::arg("formula"));
}