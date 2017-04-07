#include "labeling.h"

#include "storm/models/sparse/StateLabeling.h"

// Define python bindings
void define_labeling(py::module& m) {

    // StateLabeling
    py::class_<storm::models::sparse::StateLabeling, std::shared_ptr<storm::models::sparse::StateLabeling>>(m, "StateLabeling", "Labeling for states")
        .def("add_label", [](storm::models::sparse::StateLabeling& labeling, std::string label) {
                labeling.addLabel(label);
            }, py::arg("label"), "Add label")
        .def("get_labels", &storm::models::sparse::StateLabeling::getLabels, "Get all labels")
        .def("get_labels_of_state", &storm::models::sparse::StateLabeling::getLabelsOfState, "Get labels of given state", py::arg("state"))
        .def("contains_label", &storm::models::sparse::StateLabeling::containsLabel, "Check if the given label is contained in the labeling", py::arg("label"))
        .def("add_label_to_state", &storm::models::sparse::StateLabeling::addLabelToState, "Add label to state", py::arg("label"), py::arg("state"))
        .def("has_state_label", &storm::models::sparse::StateLabeling::getStateHasLabel, "Check if the given state has the given label", py::arg("label"), py::arg("state"))
        .def("get_states", &storm::models::sparse::StateLabeling::getStates, "Get all states which have the given label", py::arg("label"))
        .def("set_states", [](storm::models::sparse::StateLabeling& labeling, std::string const& label, storm::storage::BitVector const& states) {
                labeling.setStates(label, states);
            }, "Set all states which have the given label", py::arg("label"), py::arg("states"))
        .def("__str__", [](storm::models::sparse::StateLabeling const& labeling) {
                std::ostringstream oss;
                oss << labeling;
                return oss.str();
            })
    ;

}
