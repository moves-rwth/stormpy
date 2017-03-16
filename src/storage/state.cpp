#include "state.h"

void define_state(py::module& m) {
    
    // SparseModelStates
    py::class_<SparseModelStates<double>>(m, "SparseModelStates", "States in sparse model")
        .def("__getitem__", &SparseModelStates<double>::getState)
    ;
    py::class_<SparseModelStates<storm::RationalFunction>>(m, "SparseParametricModelStates", "States in sparse parametric model")
        .def("__getitem__", &SparseModelStates<storm::RationalFunction>::getState)
    ;
    // SparseModelState
    py::class_<SparseModelState<double>>(m, "SparseModelState", "State in sparse model")
        .def("__str__", &SparseModelState<double>::toString)
        .def_property_readonly("id", &SparseModelState<double>::getIndex, "Id")
        .def_property_readonly("labels", &SparseModelState<double>::getLabels, "Labels")
        .def_property_readonly("actions", &SparseModelState<double>::getActions, "Get actions")
    ;
    py::class_<SparseModelState<storm::RationalFunction>>(m, "SparseParametricModelState", "State in sparse parametric model")
        .def("__str__", &SparseModelState<storm::RationalFunction>::toString)
        .def_property_readonly("id", &SparseModelState<storm::RationalFunction>::getIndex, "Id")
        .def_property_readonly("labels", &SparseModelState<storm::RationalFunction>::getLabels, "Labels")
        .def_property_readonly("actions", &SparseModelState<storm::RationalFunction>::getActions, "Get actions")
    ;
    
    // SparseModelActions
    py::class_<SparseModelActions<double>>(m, "SparseModelActions", "Actions for state in sparse model")
        .def("__getitem__", &SparseModelActions<double>::getAction)
    ;
    py::class_<SparseModelActions<storm::RationalFunction>>(m, "SparseParametricModelActions", "Actions for state in sparse parametric model")
        .def("__getitem__", &SparseModelActions<storm::RationalFunction>::getAction)
    ;
    // SparseModelAction
    py::class_<SparseModelAction<double>>(m, "SparseModelAction", "Action for state in sparse model")
        .def("__str__", &SparseModelAction<double>::toString)
        .def_property_readonly("id", &SparseModelAction<double>::getIndex, "Id")
        .def_property_readonly("transitions", &SparseModelAction<double>::getTransitions, "Get transitions")
    ;
    py::class_<SparseModelAction<storm::RationalFunction>>(m, "SparseParametricModelAction", "Action for state in sparse parametric model")
        .def("__str__", &SparseModelAction<storm::RationalFunction>::toString)
        .def_property_readonly("id", &SparseModelAction<storm::RationalFunction>::getIndex, "Id")
        .def_property_readonly("transitions", &SparseModelAction<storm::RationalFunction>::getTransitions, "Get transitions")
    ;

}
