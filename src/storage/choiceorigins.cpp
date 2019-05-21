#include "choiceorigins.h"
#include "storm/storage/sparse/JaniChoiceOrigins.h"
#include "storm/storage/jani/Model.h"

void define_origins(py::module& m) {
    using ChoiceOrigins = storm::storage::sparse::ChoiceOrigins;
    py::class_<ChoiceOrigins, std::shared_ptr<ChoiceOrigins>> co(m, "ChoiceOrigins",  "This class represents the origin of choices of a model in terms of the input model spec.");
    co.def("get_identifier_info", &ChoiceOrigins::getIdentifierInfo, "identifier"_a, "human readable string")
      .def("get_choice_info", &ChoiceOrigins::getChoiceInfo, "identifier"_a, "human readable string")      ;
    using JaniChoiceOrigins = storm::storage::sparse::JaniChoiceOrigins;
    py::class_<JaniChoiceOrigins, std::shared_ptr<JaniChoiceOrigins>>(m, "JaniChoiceOrigins", "This class represents for each choice the origin in the jani spec.", co)
            .def_property_readonly("model", &JaniChoiceOrigins::getModel, "retrieves the associated JANI model")
            .def("get_edge_index_set", [](JaniChoiceOrigins const& co, uint64_t choice) -> auto& { return co.getEdgeIndexSet(choice); }, "returns the set of edges that induced the choice", py::arg("choice_index"))
       ;
}