#include "jani.h"
#include <storm/storage/jani/Model.h>
#include <storm/storage/jani/JSONExporter.h>
#include <storm/storage/expressions/ExpressionManager.h>
#include <storm/logic/RewardAccumulationEliminationVisitor.h>
#include "src/helpers.h"

using namespace storm::jani;

std::string janiToString(Model const& m) {
    std::stringstream str;
    JsonExporter::toStream(m, {}, str);
    return str.str();
}

void define_jani(py::module& m) {
    py::class_<Model, std::shared_ptr<Model>> md(m, "JaniModel", "A Jani Model");
    md.def_property_readonly("name", &Model::getName, "model name")
        .def_property_readonly("model_type", &storm::jani::Model::getModelType, "Model type")
        .def_property_readonly("automata", [](const Model& model) {return model.getAutomata();}, "get automata")
        .def_property_readonly("constants", [](const Model& model) {return model.getConstants();}, "get constants")
        .def("restrict_edges", &Model::restrictEdges, "restrict model to edges given by set", py::arg("edge_set"))
        .def_property_readonly("expression_manager", &Model::getExpressionManager, "get expression manager", pybind11::return_value_policy::reference_internal)
        .def_property_readonly("has_undefined_constants", &Model::hasUndefinedConstants, "Flag if program has undefined constants")
        .def_property_readonly("undefined_constants_are_graph_preserving", &Model::undefinedConstantsAreGraphPreserving, "Flag if the undefined constants do not change the graph structure")
        .def("__str__", &janiToString)
        .def("define_constants", &Model::defineUndefinedConstants, "define constants with a mapping from the corresponding expression variables to expressions", py::arg("map"))
        .def("substitute_constants", &Model::substituteConstants, "substitute constants")
        .def("get_automaton_index", &Model::getAutomatonIndex, "get index for automaton name")
        .def_static("encode_edge_and_automaton_index", &Model::encodeAutomatonAndEdgeIndices, "get edge/automaton-index")
        .def_static("decode_edge_and_automaton_index", &Model::decodeAutomatonAndEdgeIndices, "get edge and automaton from edge/automaton index")
    ;

    py::class_<Automaton, std::shared_ptr<Automaton>> automaton(m, "JaniAutomaton", "A Jani Automation");
    automaton.def_property_readonly("edges",[](const Automaton& a) {
            return a.getEdges();
        }, "get edges")
        .def_property_readonly("name", &Automaton::getName)
    ;

    py::class_<Edge, std::shared_ptr<Edge>> edge(m, "JaniEdge", "A Jani Edge");
    edge.def_property_readonly("source_location_index", &Edge::getSourceLocationIndex, "index for source location")
        .def_property_readonly("destinations", [](Edge const& e) {
                return e.getDestinations();
             }, "edge destinations")
        .def_property_readonly("nr_destinations", &Edge::getNumberOfDestinations, "nr edge destinations")
        .def_property_readonly("guard", &Edge::getGuard, "edge guard")
        .def("substitute", &Edge::substitute, py::arg("mapping"))
        .def("has_silent_action", &Edge::hasSilentAction, "Is the edge labelled with the silent action")
    ;

    py::class_<TemplateEdge, std::shared_ptr<TemplateEdge>> templateEdge(m, "JaniTemplateEdge", "Template edge, internal data structure for edges");
    templateEdge.def(py::init<storm::expressions::Expression>());

    py::class_<EdgeDestination, std::shared_ptr<EdgeDestination>> edgeDestination(m, "JaniEdgeDestination", "Destination in Jani");
    edgeDestination.def_property_readonly("target_location_index", &EdgeDestination::getLocationIndex)
        .def_property_readonly("probability", &EdgeDestination::getProbability)
        .def_property_readonly("assignments", &EdgeDestination::getOrderedAssignments)
    ;

    py::class_<OrderedAssignments, std::shared_ptr<OrderedAssignments>> orderedAssignments(m, "JaniOrderedAssignments", "Set of assignments");
    orderedAssignments.def("__iter__", [](OrderedAssignments &v) {
                return py::make_iterator(v.begin(), v.end());
            }, py::keep_alive<0, 1>()) /* Keep vector alive while iterator is used */
        .def("__str__", &streamToString<OrderedAssignments>)
    ;

    py::class_<Assignment, std::shared_ptr<Assignment>> assignment(m, "JaniAssignment", "Jani Assignment");
    assignment.def("__str__", &streamToString<Assignment>)
        .def_property("expression", &Assignment::getAssignedExpression, &Assignment::setAssignedExpression)
    ;

    py::class_<Constant, std::shared_ptr<Constant>> constant(m, "JaniConstant", "A Constant in JANI");
    constant.def_property_readonly("defined", &Constant::isDefined, "is constant defined by some expression")
        .def_property_readonly("name", &Constant::getName, "name of constant")
        .def_property_readonly("type", &Constant::getType, "type of constant")
        .def_property_readonly("expression_variable", &Constant::getExpressionVariable, "expression variable for this constant")
    ;


    m.def("eliminate_reward_accumulations", [](const Model& model, std::vector<Property>& properties) {
            storm::logic::RewardAccumulationEliminationVisitor v(model);
            v.eliminateRewardAccumulations(properties);
            return properties;
        }, "Eliminate reward accumulations", py::arg("model"), py::arg("properties"));



}