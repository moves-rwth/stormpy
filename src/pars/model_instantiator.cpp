#include "model_instantiator.h"
#include "storm/models/sparse/Model.h"

template<typename ValueType> using Model = storm::models::sparse::Model<ValueType>;
template<typename ValueType> using Dtmc = storm::models::sparse::Dtmc<ValueType>;
template<typename ValueType> using Mdp = storm::models::sparse::Mdp<ValueType>;
template<typename ValueType> using Ctmc = storm::models::sparse::Ctmc<ValueType>;
template<typename ValueType> using MarkovAutomaton = storm::models::sparse::MarkovAutomaton<ValueType>;

// Model instantiator
void define_model_instantiator(py::module& m) {
    py::class_<storm::utility::ModelInstantiator<Dtmc<storm::RationalFunction>, Dtmc<double>>>(m, "PDtmcInstantiator", "Instantiate PDTMCs to DTMCs")
        .def(py::init<Dtmc<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<Dtmc<storm::RationalFunction>, Dtmc<double>>::instantiate, "Instantiate model with given parameter values")
    ;

    py::class_<storm::utility::ModelInstantiator<Mdp<storm::RationalFunction>,Mdp<double>>>(m, "PMdpInstantiator", "Instantiate PMDPs to MDPs")
        .def(py::init<Mdp<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<Mdp<storm::RationalFunction>, Mdp<double>>::instantiate, "Instantiate model with given parameter values")
    ;

    py::class_<storm::utility::ModelInstantiator<Ctmc<storm::RationalFunction>,Ctmc<double>>>(m, "PCtmcInstantiator", "Instantiate PCTMCs to CTMCs")
        .def(py::init<Ctmc<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<Ctmc<storm::RationalFunction>, Ctmc<double>>::instantiate, "Instantiate model with given parameter values");

    py::class_<storm::utility::ModelInstantiator<MarkovAutomaton<storm::RationalFunction>,MarkovAutomaton<double>>>(m, "PMaInstantiator", "Instantiate PMAs to MAs")
        .def(py::init<MarkovAutomaton<storm::RationalFunction>>(), "parametric model"_a)
        .def("instantiate", &storm::utility::ModelInstantiator<MarkovAutomaton<storm::RationalFunction>, MarkovAutomaton<double>>::instantiate, "Instantiate model with given parameter values");
}
