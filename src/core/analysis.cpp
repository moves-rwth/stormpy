#include "analysis.h"

#include "storm/analysis/GraphConditions.h"

// Define python bindings
void define_graph_constraints(py::module& m) {
    py::class_<storm::analysis::ConstraintCollector<storm::RationalFunction>>(m, "ConstraintCollector", "Collects constraints on parametric Markov chains")
            .def(py::init<storm::models::sparse::Dtmc<storm::RationalFunction>>(), "model"_a)
            .def_property_readonly("wellformed_constraints", &storm::analysis::ConstraintCollector<storm::RationalFunction>::getWellformedConstraints)
            .def_property_readonly("graph_preserving_constraints", &storm::analysis::ConstraintCollector<storm::RationalFunction>::getGraphPreservingConstraints)
    ;

}
