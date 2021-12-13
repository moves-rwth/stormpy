#include "analysis.h"

#include "storm-dft/parser/DFTJsonParser.h"
#include "storm-dft/builder/ExplicitDFTModelBuilder.h"
#include "storm-dft/storage/dft/DFTIsomorphism.h"


// Thin wrapper for DFT analysis
template<typename ValueType>
std::vector<ValueType> analyzeDFT(storm::storage::DFT<ValueType> const& dft, std::vector<std::shared_ptr<storm::logic::Formula const>> const& properties, bool symred, bool allowModularisation, storm::utility::RelevantEvents const& relevantEvents, bool allowDCForRelevant) {
    typename storm::modelchecker::DFTModelChecker<ValueType>::dft_results dftResults = storm::api::analyzeDFT(dft, properties, symred, allowModularisation, relevantEvents, allowDCForRelevant, 0.0, storm::builder::ApproximationHeuristic::DEPTH, false);

    std::vector<ValueType> results;
    for (auto result : dftResults) {
        results.push_back(boost::get<ValueType>(result));
    }
    return results;
}


// Define python bindings
void define_analysis(py::module& m) {
    // RelevantEvents
    py::class_<storm::utility::RelevantEvents, std::shared_ptr<storm::utility::RelevantEvents>>(m, "RelevantEvents", "Relevant events which should be observed")
        .def("is_relevant", &storm::utility::RelevantEvents::isRelevant, "Check whether the given name is a relevant event", py::arg("name"))
    ;

    m.def("analyze_dft", &analyzeDFT<double>, "Analyze the DFT", py::arg("dft"), py::arg("properties"), py::arg("symred")=true, py::arg("allow_modularisation")=false, py::arg("relevant_events")=storm::utility::RelevantEvents(), py::arg("allow_dc_for_relevant")=false);
    m.def("analyze_parametric_dft", &analyzeDFT<storm::RationalFunction>, "Analyze the parametric DFT", py::arg("dft"), py::arg("properties"), py::arg("symred")=true, py::arg("allow_modularisation")=false, py::arg("relevant_events")=storm::utility::RelevantEvents(), py::arg("allow_dc_for_relevant")=false);

    m.def("transform_dft", &storm::api::applyTransformations<double>, "Apply transformations on DFT", py::arg("dft"), py::arg("unique_constant_be"), py::arg("binary_fdeps"));
    m.def("transform_parametric_dft", &storm::api::applyTransformations<storm::RationalFunction>, "Apply transformations on parametric DFT", py::arg("dft"), py::arg("unique_constant_be"), py::arg("binary_fdeps"));

    m.def("compute_dependency_conflicts", &storm::api::computeDependencyConflicts<double>, "Set conflicts between FDEPs. Is used in analysis.", py::arg("dft"), py::arg("use_smt") = false, py::arg("solver_timeout") = 0);
    m.def("compute_dependency_conflicts_parametric", &storm::api::computeDependencyConflicts<storm::RationalFunction>, "Set conflicts between FDEPs. Is used in analysis.", py::arg("dft"), py::arg("use_smt") = false, py::arg("solver_timeout") = 0);

    m.def("is_well_formed", &storm::api::isWellFormed<double>, "Check whether DFT is well-formed.", py::arg("dft"), py::arg("check_valid_for_analysis") = true);
    m.def("is_well_formed_parametric", &storm::api::isWellFormed<storm::RationalFunction>, "Check whether parametric DFT is well-formed.", py::arg("dft"), py::arg("check_valid_for_analysis") = true);

    m.def("compute_relevant_events", &storm::api::computeRelevantEvents<double>, "Compute relevant event ids from properties and additional relevant names", py::arg("dft"), py::arg("properties"), py::arg("additional_relevant_names") = std::vector<std::string>());
    m.def("compute_relevant_events_parametric", &storm::api::computeRelevantEvents<storm::RationalFunction>, "Compute relevant event ids from properties and additional relevant names", py::arg("dft"), py::arg("properties"), py::arg("additional_relevant_names") = std::vector<std::string>());
}
