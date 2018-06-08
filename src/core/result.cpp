#include "result.h"
#include "storm/analysis/GraphConditions.h"
#include "storm/modelchecker/results/SymbolicQualitativeCheckResult.h"
#include "storm/modelchecker/results/SymbolicQuantitativeCheckResult.h"
#include "storm/modelchecker/results/HybridQuantitativeCheckResult.h"
#include "storm/models/symbolic/StandardRewardModel.h"

// Define python bindings
void define_result(py::module& m) {

    // CheckResult
    py::class_<storm::modelchecker::CheckResult, std::shared_ptr<storm::modelchecker::CheckResult>> checkResult(m, "_CheckResult", "Base class for all modelchecking results");
    checkResult.def_property_readonly("_symbolic", &storm::modelchecker::CheckResult::isSymbolic, "Flag if result is symbolic")
        .def_property_readonly("_hybrid", &storm::modelchecker::CheckResult::isHybrid, "Flag if result is hybrid")
        .def_property_readonly("_quantitative", &storm::modelchecker::CheckResult::isQuantitative, "Flag if result is quantitative")
        .def_property_readonly("_qualitative", &storm::modelchecker::CheckResult::isQualitative, "Flag if result is qualitative")
        .def_property_readonly("_explicit_qualitative", &storm::modelchecker::CheckResult::isExplicitQualitativeCheckResult, "Flag if result is explicit qualitative")
        .def_property_readonly("_explicit_quantitative", &storm::modelchecker::CheckResult::isExplicitQuantitativeCheckResult, "Flag if result is explicit quantitative")
        .def_property_readonly("_symbolic_qualitative", &storm::modelchecker::CheckResult::isSymbolicQualitativeCheckResult, "Flag if result is symbolic qualitative")
        .def_property_readonly("_symbolic_quantitative", &storm::modelchecker::CheckResult::isSymbolicQuantitativeCheckResult, "Flag if result is symbolic quantitative")
        .def_property_readonly("_hybrid_quantitative", &storm::modelchecker::CheckResult::isHybridQuantitativeCheckResult, "Flag if result is hybrid quantitative")
        .def_property_readonly("_pareto_curve", &storm::modelchecker::CheckResult::isParetoCurveCheckResult, "Flag if result is a pareto curve")
        .def_property_readonly("result_for_all_states", &storm::modelchecker::CheckResult::isResultForAllStates, "Flag if result is for all states")
        .def_property_readonly("has_scheduler", &storm::modelchecker::CheckResult::hasScheduler, "Flag if a scheduler is present")

        .def("as_explicit_qualitative", [](storm::modelchecker::CheckResult const& result) {
                return result.asExplicitQualitativeCheckResult();
            }, "Convert into explicit qualitative result")
        .def("as_explicit_quantitative", [](storm::modelchecker::CheckResult const& result) {
                return result.asExplicitQuantitativeCheckResult<double>();
            }, "Convert into explicit quantitative result")
        .def("as_explicit_parametric_quantitative", [](storm::modelchecker::CheckResult const& result) {
                return result.asExplicitQuantitativeCheckResult<storm::RationalFunction>();
            }, "Convert into explicit quantitative result")
        .def("__str__",  [](storm::modelchecker::CheckResult const& result) {
                std::stringstream stream;
                result.writeToStream(stream);
                return stream.str();
            })
    ;

    // QualitativeCheckResult
    py::class_<storm::modelchecker::QualitativeCheckResult, std::shared_ptr<storm::modelchecker::QualitativeCheckResult>> qualitativeCheckResult(m, "_QualitativeCheckResult", "Abstract class for qualitative model checking results", checkResult);
    py::class_<storm::modelchecker::ExplicitQualitativeCheckResult, std::shared_ptr<storm::modelchecker::ExplicitQualitativeCheckResult>>(m, "ExplicitQualitativeCheckResult", "Explicit qualitative model checking result", qualitativeCheckResult)
            .def("at", [](storm::modelchecker::ExplicitQualitativeCheckResult const& result, storm::storage::sparse::state_type state) {
                return result[state];
            }, py::arg("state"), "Get result for given state")
            .def("get_truth_values", &storm::modelchecker::ExplicitQualitativeCheckResult::getTruthValuesVector, "Get BitVector representing the truth values")
    ;
    py::class_<storm::modelchecker::SymbolicQualitativeCheckResult<storm::dd::DdType::Sylvan>, std::shared_ptr<storm::modelchecker::SymbolicQualitativeCheckResult<storm::dd::DdType::Sylvan>>>(m, "SymbolicQualitativeCheckResult", "Symbolic qualitative model checking result", qualitativeCheckResult)
    ;

    // QuantitativeCheckResult
    py::class_<storm::modelchecker::QuantitativeCheckResult<double>, std::shared_ptr<storm::modelchecker::QuantitativeCheckResult<double>>> quantitativeCheckResult(m, "_QuantitativeCheckResult", "Abstract class for quantitative model checking results", checkResult);
    py::class_<storm::modelchecker::ExplicitQuantitativeCheckResult<double>, std::shared_ptr<storm::modelchecker::ExplicitQuantitativeCheckResult<double>>>(m, "ExplicitQuantitativeCheckResult", "Explicit quantitative model checking result", quantitativeCheckResult)
        .def("at", [](storm::modelchecker::ExplicitQuantitativeCheckResult<double> const& result, storm::storage::sparse::state_type state) {
            return result[state];
        }, py::arg("state"), "Get result for given state")
        .def("get_values", [](storm::modelchecker::ExplicitQuantitativeCheckResult<double> const& res) {return res.getValueVector();}, "Get model checking result values for all states")
        .def_property_readonly("scheduler", [](storm::modelchecker::ExplicitQuantitativeCheckResult<double> const& res) {return res.getScheduler();}, "get scheduler")
    ;
    py::class_<storm::modelchecker::SymbolicQuantitativeCheckResult<storm::dd::DdType::Sylvan, double>, std::shared_ptr<storm::modelchecker::SymbolicQuantitativeCheckResult<storm::dd::DdType::Sylvan, double>>>(m, "SymbolicQuantitativeCheckResult", "Symbolic quantitative model checking result", quantitativeCheckResult)
    ;
    py::class_<storm::modelchecker::HybridQuantitativeCheckResult<storm::dd::DdType::Sylvan, double>, std::shared_ptr<storm::modelchecker::HybridQuantitativeCheckResult<storm::dd::DdType::Sylvan, double>>>(m, "HybridQuantitativeCheckResult", "Hybrid quantitative model checking result", quantitativeCheckResult)
        .def("get_values", &storm::modelchecker::HybridQuantitativeCheckResult<storm::dd::DdType::Sylvan, double>::getExplicitValueVector, "Get model checking result values for all states")
    ;

    py::class_<storm::modelchecker::QuantitativeCheckResult<storm::RationalFunction>, std::shared_ptr<storm::modelchecker::QuantitativeCheckResult<storm::RationalFunction>>> parametricQuantitativeCheckResult(m, "_ParametricQuantitativeCheckResult", "Abstract class for parametric quantitative model checking results", checkResult);
    py::class_<storm::modelchecker::ExplicitQuantitativeCheckResult<storm::RationalFunction>, std::shared_ptr<storm::modelchecker::ExplicitQuantitativeCheckResult<storm::RationalFunction>>>(m, "ExplicitParametricQuantitativeCheckResult", "Explicit parametric quantitative model checking result", parametricQuantitativeCheckResult)
        .def("at", [](storm::modelchecker::ExplicitQuantitativeCheckResult<storm::RationalFunction> const& result, storm::storage::sparse::state_type state) {
            return result[state];
        }, py::arg("state"), "Get result for given state")
        .def("get_values", [](storm::modelchecker::ExplicitQuantitativeCheckResult<storm::RationalFunction> const& res) { return res.getValueVector();}, "Get model checking result values for all states")
    ;
    py::class_<storm::modelchecker::SymbolicQuantitativeCheckResult<storm::dd::DdType::Sylvan, storm::RationalFunction>, std::shared_ptr<storm::modelchecker::SymbolicQuantitativeCheckResult<storm::dd::DdType::Sylvan, storm::RationalFunction>>>(m, "SymbolicParametricQuantitativeCheckResult", "Symbolic parametric quantitative model checking result", quantitativeCheckResult)
    ;
    py::class_<storm::modelchecker::HybridQuantitativeCheckResult<storm::dd::DdType::Sylvan, storm::RationalFunction>, std::shared_ptr<storm::modelchecker::HybridQuantitativeCheckResult<storm::dd::DdType::Sylvan, storm::RationalFunction>>>(m, "HybridParametricQuantitativeCheckResult", "Symbolic parametric hybrid quantitative model checking result", quantitativeCheckResult)
        .def("get_values", &storm::modelchecker::HybridQuantitativeCheckResult<storm::dd::DdType::Sylvan, storm::RationalFunction>::getExplicitValueVector, "Get model checking result values for all states")
    ;

}

