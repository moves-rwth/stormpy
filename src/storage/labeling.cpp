#include "labeling.h"
#include "src/helpers.h"

#include "storm/models/sparse/ItemLabeling.h"
#include "storm/models/sparse/StateLabeling.h"
#include "storm/models/sparse/ChoiceLabeling.h"

// Define python bindings
void define_labeling(py::module& m) {

    // ItemLabeling
    py::class_<storm::models::sparse::ItemLabeling, std::shared_ptr<storm::models::sparse::ItemLabeling>> labeling(m,"ItemLabeling", "Labeling");
    labeling.def("add_label", [](storm::models::sparse::ItemLabeling& labeling, std::string label) {
                labeling.addLabel(label);
            }, py::arg("label"), "Add label")
        .def("get_labels", &storm::models::sparse::ItemLabeling::getLabels, "Get all labels")
        .def("contains_label", &storm::models::sparse::ItemLabeling::containsLabel, "Check if the given label is contained in the labeling", py::arg("label"))
        .def("__str__", &streamToString<storm::models::sparse::ItemLabeling>)
    ;


    // StateLabeling
    py::class_<storm::models::sparse::StateLabeling, std::shared_ptr<storm::models::sparse::StateLabeling>>(m, "StateLabeling", "Labeling for states", labeling)
        .def("get_labels_of_state", &storm::models::sparse::StateLabeling::getLabelsOfState, "Get labels of given state", py::arg("state"))
        .def("add_label_to_state", &storm::models::sparse::StateLabeling::addLabelToState, "Add label to state", py::arg("label"), py::arg("state"))
        .def("has_state_label", &storm::models::sparse::StateLabeling::getStateHasLabel, "Check if the given state has the given label", py::arg("label"), py::arg("state"))
        .def("get_states", &storm::models::sparse::StateLabeling::getStates, "Get all states which have the given label", py::arg("label"))
        .def("set_states", [](storm::models::sparse::StateLabeling& labeling, std::string const& label, storm::storage::BitVector const& states) {
                labeling.setStates(label, states);
            }, "Set all states which have the given label", py::arg("label"), py::arg("states"))
        .def("__str__", &streamToString<storm::models::sparse::StateLabeling>)
    ;


    py::class_<storm::models::sparse::ChoiceLabeling>(m, "ChoiceLabeling", "Labeling for choices", labeling).
            def("get_labels_of_choice", &storm::models::sparse::ChoiceLabeling::getLabelsOfChoice, py::arg("choice"), "get labels of a choice");
}
