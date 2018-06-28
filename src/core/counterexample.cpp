#include "counterexample.h"
#include "storm/environment/Environment.h"
#include "storm-counterexamples/api/counterexamples.h"

using namespace storm::counterexamples;

// Define python bindings
void define_counterexamples(py::module& m) {
    py::class_<boost::container::flat_set<uint_fast64_t>>(m, "FlatSet", "Container to pass to program")
            .def(py::init<>())
            .def("insert", [](boost::container::flat_set<uint_fast64_t>& flatset, uint64_t value) {flatset.insert(value);})
            .def("__str__", [](boost::container::flat_set<uint64_t> const& set) { std::stringstream str; str << "["; for(auto const& i : set) { str << i << ", ";} str << "]"; return str.str(); })
            .def("__len__", [](boost::container::flat_set<uint64_t> const& set) { return set.size();})
            ;
    using CexGeneratorOptions =  SMTMinimalLabelSetGenerator<double>::Options;
            py::class_<CexGeneratorOptions>(m, "SMTCounterExampleGeneratorOptions", "Options for highlevel counterexample generation")
                    .def(py::init<>())
                    .def_readwrite("check_threshold_feasible", &CexGeneratorOptions::checkThresholdFeasible)
                    .def_readwrite("encode_reachability", &CexGeneratorOptions::encodeReachability)
                    .def_readwrite("silent", &CexGeneratorOptions::silent)
                    .def_readwrite("use_dynamic_constraints", &CexGeneratorOptions::useDynamicConstraints)
                    .def_readwrite("maximum_counterexamples", &CexGeneratorOptions::maximumCounterexamples)
                    .def_readwrite("continue_after_first_counterexample",  &CexGeneratorOptions::continueAfterFirstCounterexampleUntil)

            ;
            py::class_<SMTMinimalLabelSetGenerator<double>>(m, "SMTCounterExampleGenerator", "Highlevel Counterexample Generator with SMT as backend").
            def_static("build", &SMTMinimalLabelSetGenerator<double>::computeCounterexampleLabelSet, "Compute counterexample", py::arg("env"), py::arg("symbolic_model"), py::arg("model"), py::arg("formula"), py::arg("dontcare"), py::arg("options"))
            ;


}
