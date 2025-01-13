#include "multiobjective.h"

#include "storm/modelchecker/multiobjective/multiObjectiveModelChecking.h"

#include "storm/adapters/RationalNumberAdapter.h"
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


template<typename ValueType>
std::pair<ValueType, ValueType> _checkForRelReach(storm::Environment const& env, storm::models::sparse::Mdp<ValueType> const& model,
                                                                storm::logic::MultiObjectiveFormula const& formula) {
    // This internal check for RelReach requires some cleanup before it makes sense to merge this.
    //    STORM_LOG_ASSERT(model.getInitialStates().getNumberOfSetBits() == 1,
    //                     "Multi-objective Model checking on model with multiple initial states is not supported.");
    std::vector<ValueType> weightVector = {storm::utility::one<ValueType>(),-storm::utility::one<ValueType>()};
    // Preprocess the model
    auto preprocessorResult = storm::modelchecker::multiobjective::preprocessing::SparseMultiObjectivePreprocessor<storm::models::sparse::Mdp<ValueType>>::preprocess(env, model, formula);
    auto checker = storm::modelchecker::multiobjective::StandardMdpPcaaWeightVectorChecker(preprocessorResult);
    checker.check(env, weightVector);
    ValueType underApprox = checker.getUnderApproximationOfInitialStateResults().at(0) - checker.getOverApproximationOfInitialStateResults().at(1);
    ValueType overApprox = checker.getOverApproximationOfInitialStateResults().at(0) - checker.getUnderApproximationOfInitialStateResults().at(1);

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
    m.def("compute_rel_reach_helper", &_checkForRelReach<double>, py::arg("env"), py::arg("model"), py::arg("formula"));
    m.def("compute_rel_reach_helper_exact", &_checkForRelReach<storm::RationalNumber>, py::arg("env"), py::arg("model"), py::arg("formula"));
}