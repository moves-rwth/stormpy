#include "pla.h"
#include "src/helpers.h"
#include "storm/api/storm.h"


typedef storm::modelchecker::SparseDtmcParameterLiftingModelChecker<storm::models::sparse::Dtmc<storm::RationalFunction>, double> DtmcParameterLiftingModelChecker;
typedef storm::modelchecker::SparseMdpParameterLiftingModelChecker<storm::models::sparse::Mdp<storm::RationalFunction>, double> MdpParameterLiftingModelChecker;

typedef storm::modelchecker::RegionModelChecker<storm::RationalFunction> RegionModelChecker;
typedef storm::storage::ParameterRegion<storm::RationalFunction> Region;

// Thin wrappers
std::shared_ptr<RegionModelChecker> createRegionChecker(storm::Environment const& env, std::shared_ptr<storm::models::sparse::Model<storm::RationalFunction>> const& model, std::shared_ptr<storm::logic::Formula> const& formula, bool generateSplittingEstimate, bool allowModelSimplifications) {
    return storm::api::initializeParameterLiftingRegionModelChecker<storm::RationalFunction, double>(env, model, storm::api::createTask<storm::RationalFunction>(formula, true), generateSplittingEstimate, allowModelSimplifications);
}

void specify(std::shared_ptr<RegionModelChecker>& checker, storm::Environment const& env, std::shared_ptr<storm::models::sparse::Model<storm::RationalFunction>> const& model, std::shared_ptr<storm::logic::Formula> const& formula, bool generateSplittingEstimate, bool allowModelSimplifications) {
    return checker->specify(env, model, storm::api::createTask<storm::RationalFunction>(formula, true), generateSplittingEstimate, allowModelSimplifications);
}

storm::modelchecker::RegionResult checkRegion(std::shared_ptr<RegionModelChecker>& checker, storm::Environment const& env, Region const& region, storm::modelchecker::RegionResultHypothesis const& hypothesis, storm::modelchecker::RegionResult const& initialResult, bool sampleVertices) {
    return checker->analyzeRegion(env, region, hypothesis, initialResult, sampleVertices);
}

storm::RationalFunction getBoundAtInit(std::shared_ptr<RegionModelChecker>& checker, storm::Environment const& env, Region const& region, bool maximise) {
    return checker->getBoundAtInitState(env, region, maximise ?  storm::solver::OptimizationDirection::Maximize : storm::solver::OptimizationDirection::Minimize);
}

storm::modelchecker::ExplicitQuantitativeCheckResult<double> getBound_dtmc(std::shared_ptr<DtmcParameterLiftingModelChecker>& checker, storm::Environment const& env, Region const& region, bool maximise) {
    return checker->getBound(env, region, maximise ?  storm::solver::OptimizationDirection::Maximize : storm::solver::OptimizationDirection::Minimize)->asExplicitQuantitativeCheckResult<double>();
}

storm::modelchecker::ExplicitQuantitativeCheckResult<double> getBound_mdp(std::shared_ptr<MdpParameterLiftingModelChecker>& checker, storm::Environment const& env, Region const& region, bool maximise) {
    return checker->getBound(env, region, maximise ?  storm::solver::OptimizationDirection::Maximize : storm::solver::OptimizationDirection::Minimize)->asExplicitQuantitativeCheckResult<double>();
}

std::set<storm::Polynomial> gatherDerivatives(storm::models::sparse::Model<storm::RationalFunction> const& model, carl::Variable const& var) {
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
        .def("__init__", [](Region &instance, std::map<Region::VariableType, std::pair<Region::CoefficientType, Region::CoefficientType>> valuation) -> void {
                Region::Valuation lowerValuation;
                Region::Valuation upperValuation;
                for (auto const& val : valuation) {
                    lowerValuation[val.first] = val.second.first;
                    upperValuation[val.first] = val.second.second;
                }
                new (&instance) Region(lowerValuation, upperValuation);
            }, "Create region from valuation of var -> (lower_bound, upper_bound)", py::arg("valuation"))
        .def_static("create_from_string", [](std::string const& regionString, std::set<Region::VariableType> const& variables) -> Region {
                return storm::api::parseRegion<storm::RationalFunction>(regionString, variables);
            }, "Create region from string", py::arg("region_string"), py::arg("variables"))
        .def_property_readonly("area", &Region::area, "Get area")
        .def("__str__", &streamToString<Region>)
    ;

    // RegionModelChecker
    py::class_<RegionModelChecker, std::shared_ptr<RegionModelChecker>> regionModelChecker(m, "RegionModelChecker", "Region model checker via paramater lifting");
    regionModelChecker.def("check_region", &checkRegion, "Check region", py::arg("environment"), py::arg("region"), py::arg("hypothesis") = storm::modelchecker::RegionResultHypothesis::Unknown, py::arg("initialResult") = storm::modelchecker::RegionResult::Unknown, py::arg("sampleVertices") = false)
        .def("get_bound", &getBoundAtInit, "Get bound", py::arg("environment"), py::arg("region"), py::arg("maximise")= true)
        .def("get_split_suggestion", &RegionModelChecker::getRegionSplitEstimate, "Get estimate")
        .def("specify", &specify, "specify arguments",py::arg("environment"), py::arg("model"), py::arg("formula"), py::arg("generate_splitting_estimate") = false, py::arg("allow_model_simplification") = true);
    ;


    py::class_<DtmcParameterLiftingModelChecker, std::shared_ptr<DtmcParameterLiftingModelChecker>>(m, "DtmcParameterLiftingModelChecker", "Region model checker for DTMCs", regionModelChecker)
            .def(py::init<>())
            .def("get_bound_all_states", &getBound_dtmc, "Get bound", py::arg("environment"), py::arg("region"), py::arg("maximise")= true);
    py::class_<MdpParameterLiftingModelChecker, std::shared_ptr<MdpParameterLiftingModelChecker>>(m, "MdpParameterLiftingModelChecker", "Region model checker for MPDs", regionModelChecker)
            .def(py::init<>())
            .def("get_bound_all_states", &getBound_mdp, "Get bound", py::arg("environment"), py::arg("region"), py::arg("maximise")= true);

    m.def("create_region_checker", &createRegionChecker, "Create region checker", py::arg("environment"), py::arg("model"), py::arg("formula"), py::arg("generate_splitting_estimate") = false, py::arg("allow_model_simplification") = true);
    //m.def("is_parameter_lifting_sound", &storm::utility::parameterlifting::validateParameterLiftingSound, "Check if parameter lifting is sound", py::arg("model"), py::arg("formula"));
    m.def("gather_derivatives", &gatherDerivatives, "Gather all derivatives of transition probabilities", py::arg("model"), py::arg("var"));
}
