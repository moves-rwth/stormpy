#include "valuation.h"
#include "src/helpers.h"

#include "storm/storage/sparse/StateValuations.h"
#include "storm/storage/expressions/SimpleValuation.h"
#include "storm/storage/expressions/Variable.h"

// Define python bindings
void define_statevaluation(py::module& m) {

    py::class_<storm::storage::sparse::StateValuations, std::shared_ptr<storm::storage::sparse::StateValuations>> statevaluation(m,"StateValuation", "Valuations for explicit states");
    statevaluation.def("get_state", &storm::storage::sparse::StateValuations::getStateValuation)
            ;

}

void define_simplevaluation(py::module& m) {
    py::class_<storm::expressions::SimpleValuation, std::shared_ptr<storm::expressions::SimpleValuation>> simplevaluation(m, "SimpleValuation",  "Valuations for storm variables");
    simplevaluation.def("get_boolean_value", &storm::expressions::SimpleValuation::getBooleanValue);
    simplevaluation.def("get_integer_value", &storm::expressions::SimpleValuation::getIntegerValue);
    simplevaluation.def("__str__", &storm::expressions::SimpleValuation::toString);
}
