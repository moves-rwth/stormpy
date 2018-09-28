#include "prism.h"
#include <storm/storage/prism/Program.h>
#include "src/helpers.h"
#include <storm/storage/expressions/ExpressionManager.h>
#include <storm/storage/jani/Model.h>
#include <storm/storage/jani/Property.h>

using namespace storm::prism;

void define_prism(py::module& m) {
    py::class_<storm::prism::Program, std::shared_ptr<storm::prism::Program>> program(m, "PrismProgram", "A Prism Program");
    program.def_property_readonly("constants", &Program::getConstants, "Get Program Constants")
    .def_property_readonly("nr_modules", &storm::prism::Program::getNumberOfModules, "Number of modules")
            .def_property_readonly("modules", &storm::prism::Program::getModules, "Modules in the program")
            .def_property_readonly("model_type", &storm::prism::Program::getModelType, "Model type")
            .def_property_readonly("has_undefined_constants", &storm::prism::Program::hasUndefinedConstants, "Flag if program has undefined constants")
            .def_property_readonly("undefined_constants_are_graph_preserving", &storm::prism::Program::undefinedConstantsAreGraphPreserving, "Flag if the undefined constants do not change the graph structure")
            .def("substitute_constants", &Program::substituteConstants, "Substitute constants within program")
            .def("define_constants", &Program::defineUndefinedConstants, "Define constants")
            .def("restrict_commands", &Program::restrictCommands, "Restrict commands")
            .def("simplify", &Program::simplify, "Simplify")
            .def("used_constants",&Program::usedConstants, "Compute Used Constants")
            .def_property_readonly("expression_manager", &Program::getManager, "Get the expression manager for expressions in this program")
            .def("to_jani", [](storm::prism::Program const& program, std::vector<storm::jani::Property> const& properties, bool allVariablesGlobal, std::string suffix) {
                    return program.toJani(properties, allVariablesGlobal, suffix);
                }, "Transform to Jani program", py::arg("properties"), py::arg("all_variables_global") = true, py::arg("suffix") = "")
            .def("__str__", &streamToString<storm::prism::Program>);

    py::class_<Module> module(m, "PrismModule", "A module in a Prism program");
    module.def_property_readonly("commands", [](Module const& module) {return module.getCommands();}, "Commands in the module")
            .def_property_readonly("name", &Module::getName, "Name of the module")
                    ;

    py::class_<Command> command(m, "PrismCommand", "A command in a Prism program");
    command.def_property_readonly("global_index", &Command::getGlobalIndex, "Get global index")
            .def_property_readonly("guard_expression", &Command::getGuardExpression, "Get guard expression")
            .def_property_readonly("updates", &Command::getUpdates, "Updates in the command");

    py::class_<Update> update(m, "PrismUpdate", "An update in a Prism command");
    update.def_property_readonly("assignments", &Update::getAssignments, "Assignments in the update");

    py::class_<Assignment> assignment(m, "PrismAssignment", "An assignment in prism");
    assignment.def_property_readonly("variable", &Assignment::getVariable, "Variable that is updated")
            .def_property_readonly("expression", &Assignment::getExpression, "Expression for the update");
    


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
            .def_property_readonly("type", &Constant::getType, "The type of the constant")
            .def_property_readonly("expression_variable", &Constant::getExpressionVariable, "Expression variable")
            ;


}