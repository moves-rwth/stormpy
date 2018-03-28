#include "counterexample.h"
#include "storm/environment/Environment.h"

using namespace storm::counterexamples;

// Define python bindings
void define_counterexamples(py::module& m) {
    py::class_<boost::container::flat_set<uint_fast64_t>>(m, "FlatSet", "Container to pass to program");


            py::class_<SMTMinimalLabelSetGenerator<double>>(m, "SMTCounterExampleGenerator", "Highlevel Counterexample Generator with SMT as backend").
            def_static("build", &SMTMinimalLabelSetGenerator<double>::computeCounterexampleLabelSet)
            ;


}
