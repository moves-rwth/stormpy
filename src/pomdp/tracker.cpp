#include "tracker.h"
#include "src/helpers.h"
#include <storm-pomdp/generator/BeliefSupportTracker.h>


template<typename ValueType> using SparsePomdp = storm::models::sparse::Pomdp<ValueType>;
template<typename ValueType> using SparsePomdpTracker = storm::generator::BeliefSupportTracker<double>;


void define_tracker(py::module& m) {
    py::class_<storm::generator::BeliefSupportTracker<double>> tracker(m, "BeliefSupportTrackerDouble", "Tracker for BeliefSupports");
    tracker.def(py::init<SparsePomdp<double> const&>(), py::arg("pomdp"));
    tracker.def("get_current_belief_support", &SparsePomdpTracker<double>::getCurrentBeliefSupport, "What is the support given the trace so far");
    tracker.def("track", &SparsePomdpTracker<double>::track, py::arg("action"), py::arg("observation"));
}