#include "tracker.h"
#include "src/helpers.h"
#include <storm-pomdp/generator/BeliefSupportTracker.h>
#include <storm-pomdp/generator/NondeterministicBeliefTracker.h>


template<typename ValueType> using SparsePomdp = storm::models::sparse::Pomdp<ValueType>;
template<typename ValueType> using SparsePomdpTracker = storm::generator::BeliefSupportTracker<ValueType>;

template<typename ValueType> using NDPomdpTrackerSparse = storm::generator::NondeterministicBeliefTracker<double, storm::generator::SparseBeliefState<double>>;
template<typename ValueType> using NDPomdpTrackerDense = storm::generator::NondeterministicBeliefTracker<double, storm::generator::ObservationDenseBeliefState<double>>;


void define_tracker(py::module& m) {
    py::class_<storm::generator::BeliefSupportTracker<double>> tracker(m, "BeliefSupportTrackerDouble", "Tracker for BeliefSupports");
    tracker.def(py::init<SparsePomdp<double> const&>(), py::arg("pomdp"));
    tracker.def("get_current_belief_support", &SparsePomdpTracker<double>::getCurrentBeliefSupport, "What is the support given the trace so far");
    tracker.def("track", &SparsePomdpTracker<double>::track, py::arg("action"), py::arg("observation"));

    py::class_<storm::generator::SparseBeliefState<double>> sbel(m, "SparseBeliefStateDouble", "Belief state in sparse format");
    sbel.def("get", &storm::generator::SparseBeliefState<double>::get, py::arg("state"));
    sbel.def_property_readonly("risk", &storm::generator::SparseBeliefState<double>::getRisk);
    sbel.def("__str__", &storm::generator::SparseBeliefState<double>::toString);
    sbel.def_property_readonly("is_valid", &storm::generator::SparseBeliefState<double>::isValid);
//
//    py::class_<storm::generator::ObservationDenseBeliefState<double>> dbel(m, "DenseBeliefStateDouble", "Belief state in dense format");
//    dbel.def("get", &storm::generator::ObservationDenseBeliefState<double>::get, py::arg("state"));
//    dbel.def_property_readonly("risk", &storm::generator::ObservationDenseBeliefState<double>::getRisk);
//    dbel.def("__str__", &storm::generator::ObservationDenseBeliefState<double>::toString);

    py::class_<NDPomdpTrackerSparse<double>> ndetbelieftracker(m, "NondeterministicBeliefTrackerDoubleSparse", "Tracker for belief states and uncontrollable actions");
    ndetbelieftracker.def(py::init<SparsePomdp<double> const&>(), py::arg("pomdp"));
    ndetbelieftracker.def("reset", &NDPomdpTrackerSparse<double>::reset);
    ndetbelieftracker.def("set_risk", &NDPomdpTrackerSparse<double>::setRisk, py::arg("risk"));
    ndetbelieftracker.def("obtain_current_risk",&NDPomdpTrackerSparse<double>::getCurrentRisk, py::arg("max")=true);
    ndetbelieftracker.def("track", &NDPomdpTrackerSparse<double>::track, py::arg("observation"));
    ndetbelieftracker.def("obtain_beliefs", &NDPomdpTrackerSparse<double>::getCurrentBeliefs);
    ndetbelieftracker.def("obtain_last_observation", &NDPomdpTrackerSparse<double>::getCurrentObservation);
    ndetbelieftracker.def("reduce",&NDPomdpTrackerSparse<double>::reduce);


//    py::class_<NDPomdpTrackerDense<double>> ndetbelieftrackerd(m, "NondeterministicBeliefTrackerDoubleDense", "Tracker for belief states and uncontrollable actions");
//    ndetbelieftrackerd.def(py::init<SparsePomdp<double> const&>(), py::arg("pomdp"));
//    ndetbelieftrackerd.def("reset", &NDPomdpTrackerDense<double>::reset);
//    ndetbelieftrackerd.def("set_risk", &NDPomdpTrackerDense<double>::setRisk, py::arg("risk"));
//    ndetbelieftrackerd.def("obtain_current_risk",&NDPomdpTrackerDense<double>::getCurrentRisk, py::arg("max")=true);
//    ndetbelieftrackerd.def("track", &NDPomdpTrackerDense<double>::track, py::arg("observation"));
//    ndetbelieftrackerd.def("obtain_beliefs", &NDPomdpTrackerDense<double>::getCurrentBeliefs);
//    ndetbelieftrackerd.def("obtain_last_observation", &NDPomdpTrackerDense<double>::getCurrentObservation);
//    ndetbelieftrackerd.def("reduce",&NDPomdpTrackerDense<double>::reduce);

}