#include "bisimulation.h"

// Thin wrapper for bisimulation
template<typename ValueType>
std::shared_ptr<storm::models::sparse::Model<ValueType>> performBisimulation(std::shared_ptr<storm::models::sparse::Model<ValueType>> const& model, std::vector<std::shared_ptr<storm::logic::Formula const>> const& formulas, storm::storage::BisimulationType bisimulationType) {
    return storm::performBisimulationMinimization<storm::models::sparse::Model<ValueType>>(model, formulas, bisimulationType);
}

// Define python bindings
void define_bisimulation(py::module& m) {

    // Bisimulation
    m.def("_perform_bisimulation", &performBisimulation<double>, "Perform bisimulation", py::arg("model"), py::arg("formulas"), py::arg("bisimulation_type"));
    m.def("_perform_parametric_bisimulation", &performBisimulation<storm::RationalFunction>, "Perform bisimulation on parametric model", py::arg("model"), py::arg("formulas"), py::arg("bisimulation_type"));

    // BisimulationType 
    py::enum_<storm::storage::BisimulationType>(m, "BisimulationType", "Types of bisimulation")
        .value("STRONG", storm::storage::BisimulationType::Strong)
        .value("WEAK", storm::storage::BisimulationType::Weak)
    ;

}
