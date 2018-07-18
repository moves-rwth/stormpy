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

    using CexGeneratorStats = SMTMinimalLabelSetGenerator<double>::GeneratorStats;

    py::class_<CexGeneratorStats>(m, "SMTCounterExampleGeneratorStats", "Stats for highlevel counterexample generation")
            .def(py::init<>())
            .def_readonly("analysis_time", &CexGeneratorStats::analysisTime)
            .def_readonly("setup_time", &CexGeneratorStats::setupTime)
            .def_readonly("model_checking_time", &CexGeneratorStats::modelCheckingTime)
            .def_readonly("solver_time", &CexGeneratorStats::solverTime);

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
                def_static("precompute", &SMTMinimalLabelSetGenerator<double>::precompute, "Precompute input for counterexample generation", py::arg("env"), py::arg("symbolic_model"), py::arg("model"), py::arg("formula")).
                def_static("build", &SMTMinimalLabelSetGenerator<double>::computeCounterexampleLabelSet, "Compute counterexample", py::arg("env"), py::arg("stats"), py::arg("symbolic_model"), py::arg("model"), py::arg("cex_input"), py::arg("dontcare"), py::arg("options"))


            ;

    using CexInput = SMTMinimalLabelSetGenerator<double>::CexInput;
        py::class_<CexInput>(m, "SMTCounterExampleInput", "Precomputed input for counterexample generation")
                .def("add_reward_and_threshold", &CexInput::addRewardThresholdCombination, "add another reward structure and threshold", py::arg("reward_name"), py::arg("threshold"));



}
