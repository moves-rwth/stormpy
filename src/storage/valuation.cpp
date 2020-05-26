#include "valuation.h"
#include "src/helpers.h"

#include "storm/storage/sparse/StateValuations.h"
#include "storm/storage/expressions/SimpleValuation.h"
#include "storm/storage/expressions/Variable.h"

// Thin wrappers
std::string toJson(storm::storage::sparse::StateValuations const& valuations, storm::storage::sparse::state_type const& stateIndex, boost::optional<std::set<storm::expressions::Variable>> const& selectedVariables) {
    return valuations.toJson(stateIndex, selectedVariables).dump();
}


// Define python bindings
void define_statevaluation(py::module& m) {

    py::class_<storm::storage::sparse::StateValuations, std::shared_ptr<storm::storage::sparse::StateValuations>>(m,"StateValuation", "Valuations for explicit states")
        .def("get_boolean_value", &storm::storage::sparse::StateValuations::getBooleanValue, py::arg("state"), py::arg("variable"))
        .def("get_integer_value", &storm::storage::sparse::StateValuations::getIntegerValue, py::arg("state"), py::arg("variable"))
        .def("get_rational_value", &storm::storage::sparse::StateValuations::getRationalValue, py::arg("state"), py::arg("variable"))
        .def("get_string", &storm::storage::sparse::StateValuations::toString, py::arg("state"), py::arg("pretty")=true, py::arg("selected_variables")=boost::none)
        .def("get_json", &toJson, py::arg("state"), py::arg("selected_variables")=boost::none)
    ;

}
