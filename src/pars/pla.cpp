#include "pla.h"
#include "src/helpers.h"
#include "storm/api/storm.h"

typedef storm::modelchecker::SparseDtmcParameterLiftingModelChecker<storm::models::sparse::Dtmc<storm::RationalFunction>, double> SparseDtmcRegionChecker;
typedef storm::modelchecker::RegionModelChecker<storm::RationalFunction> RegionModelChecker;
typedef storm::storage::ParameterRegion<storm::RationalFunction> Region;

// Thin wrappers
std::shared_ptr<RegionModelChecker> createRegionChecker(std::shared_ptr<storm::models::sparse::Model<storm::RationalFunction>> const& model, std::shared_ptr<storm::logic::Formula> const& formula) {
    return storm::api::initializeParameterLiftingRegionModelChecker<storm::RationalFunction, double>(model, storm::api::createTask<storm::RationalFunction>(formula, true));
}

storm::modelchecker::RegionResult checkRegion(std::shared_ptr<RegionModelChecker>& checker, Region const& region, storm::modelchecker::RegionResultHypothesis const& hypothesis, storm::modelchecker::RegionResult const& initialResult, bool sampleVertices) {
    return checker->analyzeRegion(region, hypothesis, initialResult, sampleVertices);
}

storm::RationalFunction getBound(std::shared_ptr<RegionModelChecker>& checker, Region const& region, bool maximise) {
    return checker->getBoundAtInitState(region, maximise ?  storm::solver::OptimizationDirection::Maximize : storm::solver::OptimizationDirection::Minimize);
}


std::set<storm::Polynomial> gatherDerivatives(storm::models::sparse::Dtmc<storm::RationalFunction> const& model, carl::Variable const& var) {
    std::set<storm::Polynomial> derivatives;
    for (auto it : model.getTransitionMatrix()) {
        storm::Polynomial pol = it.getValue().derivative(var, 1).nominator();
        if (!pol.isConstant()) {
            derivatives.insert(pol);
        }
    }
    return derivatives;
}



// Define python bindings
void define_pla(py::module& m) {

    // RegionResult
    py::enum_<storm::modelchecker::RegionResult>(m, "RegionResult", "Types of region check results")
        .value("EXISTSSAT", storm::modelchecker::RegionResult::ExistsSat)
        .value("EXISTSVIOLATED", storm::modelchecker::RegionResult::ExistsViolated)
        .value("EXISTSBOTH", storm::modelchecker::RegionResult::ExistsBoth)
        .value("CENTERSAT", storm::modelchecker::RegionResult::CenterSat)
        .value("CENTERVIOLATED", storm::modelchecker::RegionResult::CenterViolated)
        .value("ALLSAT", storm::modelchecker::RegionResult::AllSat)
        .value("ALLVIOLATED", storm::modelchecker::RegionResult::AllViolated)
        .value("UNKNOWN", storm::modelchecker::RegionResult::Unknown)
        .def("__str__", &streamToString<storm::modelchecker::RegionResult>)
    ;

    // RegionResultHypothesis
    py::enum_<storm::modelchecker::RegionResultHypothesis>(m, "RegionResultHypothesis", "Hypothesis for the result of a parameter region")
        .value("UNKNOWN", storm::modelchecker::RegionResultHypothesis::Unknown)
        .value("ALLSAT", storm::modelchecker::RegionResultHypothesis::AllSat)
        .value("ALLVIOLATED", storm::modelchecker::RegionResultHypothesis::AllViolated)
        .def("__str__", &streamToString<storm::modelchecker::RegionResultHypothesis>)
    ;

    // Region
    py::class_<Region, std::shared_ptr<Region>>(m, "ParameterRegion", "Parameter region")
       .def("__init__", [](Region &instance, std::string const& regionString, std::set<Region::VariableType> const& variables) -> void {
                new (&instance) Region(storm::api::parseRegion<storm::RationalFunction>(regionString, variables));
            })
    ;

    // RegionModelChecker
    //py::class_<SparseDtmcRegionChecker, std::shared_ptr<SparseDtmcRegionChecker>>(m, "SparseDtmcRegionChecker", "Region model checker for sparse DTMCs")
    py::class_<RegionModelChecker, std::shared_ptr<RegionModelChecker>>(m, "RegionModelChecker", "Region model checker via paramater lifting")
/*        .def("__init__", [](std::unique_ptr<SparseDtmcRegionChecker>& instance, std::shared_ptr<storm::models::sparse::Dtmc<storm::RationalFunction>> model, storm::modelchecker::CheckTask<storm::logic::Formula, storm::RationalFunction> const& task) -> void {
                // Better use storm::api::initializeParameterLiftingRegionModelChecker<storm::RationalFunction, double>(model, task);
                //SparseDtmcRegionChecker tmp;
                //tmp.specify(model, task);
                auto tmp = storm::api::initializeParameterLiftingRegionModelChecker<storm::RationalFunction, double>(model, task);
                new (&instance) std::unique_ptr<SparseDtmcRegionChecker>(tmp);
            }, py::arg("model"), py::arg("task")*/
        .def("check_region", &checkRegion, "Check region", py::arg("region"), py::arg("hypothesis") = storm::modelchecker::RegionResultHypothesis::Unknown, py::arg("initialResult") = storm::modelchecker::RegionResult::Unknown, py::arg("sampleVertices") = false)
        .def("get_bound", &getBound, "Get bound", py::arg("region"), py::arg("maximise")= true);
    ;

    m.def("create_region_checker", &createRegionChecker, "Create region checker", py::arg("model"), py::arg("formula"));
    //m.def("is_parameter_lifting_sound", &storm::utility::parameterlifting::validateParameterLiftingSound, "Check if parameter lifting is sound", py::arg("model"), py::arg("formula"));
    m.def("gather_derivatives", &gatherDerivatives, "Gather all derivatives of transition probabilities", py::arg("model"), py::arg("var"));
}
