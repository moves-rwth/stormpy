#include "bisimulation.h"

// Define python bindings
void define_bisimulation(py::module& m) {

    // Bisimulation
    m.def("_perform_bisimulation", &storm::api::performBisimulationMinimization<double>, "Perform bisimulation", py::arg("model"), py::arg("formulas"), py::arg("bisimulation_type"));
    m.def("_perform_parametric_bisimulation", &storm::api::performBisimulationMinimization<storm::RationalFunction>, "Perform bisimulation on parametric model", py::arg("model"), py::arg("formulas"), py::arg("bisimulation_type"));

    // BisimulationType
    py::enum_<storm::storage::BisimulationType>(m, "BisimulationType", "Types of bisimulation")
        .value("STRONG", storm::storage::BisimulationType::Strong)
        .value("WEAK", storm::storage::BisimulationType::Weak)
    ;

}
