#include "prism.h"
#include <storm/storage/prism/Program.h>
#include "src/helpers.h"

using namespace storm::prism;

void define_prism(py::module& m) {
    py::class_<storm::prism::Program, std::shared_ptr<storm::prism::Program>> program(m, "PrismProgram", "A Prism Program");
    program.def_property_readonly("constants", &Program::getConstants, "Get Program Constants")
    .def_property_readonly("nr_modules", &storm::prism::Program::getNumberOfModules, "Number of modules")
            .def_property_readonly("model_type", &storm::prism::Program::getModelType, "Model type")
            .def_property_readonly("has_undefined_constants", &storm::prism::Program::hasUndefinedConstants, "Flag if program has undefined constants")
            .def_property_readonly("undefined_constants_are_graph_preserving", &storm::prism::Program::undefinedConstantsAreGraphPreserving, "Flag if the undefined constants do not change the graph structure")
            .def("__str__", &streamToString<storm::prism::Program>);


    // PrismType
    py::enum_<storm::prism::Program::ModelType>(m, "PrismModelType", "Type of the prism model")
            .value("DTMC", storm::prism::Program::ModelType::DTMC)
            .value("CTMC", storm::prism::Program::ModelType::CTMC)
            .value("MDP", storm::prism::Program::ModelType::MDP)
            .value("CTMDP", storm::prism::Program::ModelType::CTMDP)
            .value("MA", storm::prism::Program::ModelType::MA)
            .value("UNDEFINED", storm::prism::Program::ModelType::UNDEFINED)
            ;


    py::class_<Constant, std::shared_ptr<Constant>> constant(m, "Constant", "A constant in a Prism program");
    constant.def_property_readonly("name", &Constant::getName, "Constant name")
            .def_property_readonly("defined", &Constant::isDefined, "Is the constant defined?")
            .def_property_readonly("type", &Constant::getType, "The type of the constant");

}