#include "valuation.h"
#include "src/helpers.h"

#include "storm/storage/sparse/StateValuations.h"
#include "storm/storage/expressions/Variable.h"
#include "storm/storage/expressions/ExpressionManager.h"

// Thin wrappers
std::string toJson(storm::storage::sparse::StateValuations const& valuations, storm::storage::sparse::state_type const& stateIndex, boost::optional<std::set<storm::expressions::Variable>> const& selectedVariables) {
    return valuations.toJson(stateIndex, selectedVariables).dump();
}

void add_state(storm::storage::sparse::StateValuationsBuilder& builder, storm::storage::sparse::state_type const& state, std::vector<bool>&& booleanValues, std::vector<int64_t>&& integerValues, std::vector<storm::RationalNumber>&& rationalValues) {
    return builder.addState(state, std::move(booleanValues), std::move(integerValues), std::move(rationalValues));
}


// Define python bindings
void define_statevaluation(py::module& m) {

    py::class_<storm::storage::sparse::StateValuations, std::shared_ptr<storm::storage::sparse::StateValuations>>(m,"StateValuation", "Valuations for explicit states")
        .def("get_boolean_value", &storm::storage::sparse::StateValuations::getBooleanValue, py::arg("state"), py::arg("variable"))
        .def("get_integer_value", &storm::storage::sparse::StateValuations::getIntegerValue, py::arg("state"), py::arg("variable"))
        .def("get_rational_value", &storm::storage::sparse::StateValuations::getRationalValue, py::arg("state"), py::arg("variable"))
        .def("get_string", &storm::storage::sparse::StateValuations::toString, py::arg("state"), py::arg("pretty")=true, py::arg("selected_variables")=boost::none)
        .def("get_json", &toJson, py::arg("state"), py::arg("selected_variables")=boost::none)
        .def("get_nr_of_states", &storm::storage::sparse::StateValuations::getNumberOfStates);
    ;


    py::class_<storm::storage::sparse::StateValuationsBuilder, std::shared_ptr<storm::storage::sparse::StateValuationsBuilder>>(m,"StateValuationsBuilder")
            .def(py::init<>())
            .def("add_variable", &storm::storage::sparse::StateValuationsBuilder::addVariable, py::arg("variable"), "Adds a new variable")
            .def("add_state", &add_state, py::arg("state"), py::arg("boolean_values") = std::vector<bool>(), py::arg("integer_values") = std::vector<int64_t>(), py::arg("rational_values") = std::vector<storm::RationalNumber>(), "Adds a new state, no more variables should be added afterwards")
            .def("build", &storm::storage::sparse::StateValuationsBuilder::build, "Creates the finalized state valuations object")
            ;


}
