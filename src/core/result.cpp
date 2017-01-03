#include "result.h"

// Define python bindings
void define_result(py::module& m) {

    // PmcResult
    py::class_<PmcResult, std::shared_ptr<PmcResult>>(m, "PmcResult", "Holds the results after parametric model checking")
        .def("__str__", &PmcResult::toString)
        .def_property_readonly("result_function", &PmcResult::getResultFunction, "Result as rational function")
        .def_property_readonly("constraints_well_formed", &PmcResult::getConstraintsWellFormed, "Constraints ensuring well-formed probabilities")
        .def_property_readonly("constraints_graph_preserving", &PmcResult::getConstraintsGraphPreserving, "Constraints ensuring graph preservation")
    ;

}
