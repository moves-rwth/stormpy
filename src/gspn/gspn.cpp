#include "gspn.h"
#include "src/helpers.h"
#include "storm-gspn/storage/gspn/GSPN.h"
#include "storm-gspn/storage/gspn/GspnBuilder.h"
#include "storm/settings/SettingsManager.h"

#include "storm-gspn/parser/GspnParser.h"
#include "storm/utility/file.h"

using GSPN = storm::gspn::GSPN;
using GSPNBuilder = storm::gspn::GspnBuilder;
using LayoutInfo = storm::gspn::LayoutInfo;
using Place = storm::gspn::Place;
using TimedTransition = storm::gspn::TimedTransition<GSPN::RateType>;
using ImmediateTransition = storm::gspn::ImmediateTransition<GSPN::WeightType>;
using Transition = storm::gspn::Transition;
using TransitionPartition = storm::gspn::TransitionPartition;
using GSPNParser = storm::parser::GspnParser;


void gspnToFile(GSPN const& gspn, std::string const& filepath, std::string const& option) {
    std::ofstream fs;
    storm::utility::openFile(filepath, fs);

    if(option == "to-pnpro") {
        gspn.toPnpro(fs);
    }
    else if (option == "to-pnml"){
        gspn.toPnml(fs);
    }
    storm::utility::closeFile(fs);
}


void define_gspn(py::module& m) {

    // GSPN_Builder class
    py::class_<GSPNBuilder, std::shared_ptr<GSPNBuilder>>(m, "GSPNBuilder", "Generalized Stochastic Petri Net Builder")
        .def(py::init(), "Constructor")
        .def("set_name", &GSPNBuilder::setGspnName, "Set name of GSPN", "name"_a)

        .def("add_place", &GSPNBuilder::addPlace, "Add a place to the GSPN", py::arg("capacity") = 1, py::arg("initialTokens") = 0, py::arg("name") = "")
        .def("set_place_layout_info", &GSPNBuilder::setPlaceLayoutInfo, "Set place layout information", py::arg("place_id"), py::arg("layout_info"))

        .def("add_immediate_transition", &GSPNBuilder::addImmediateTransition, "Add an immediate transition to the GSPN", "priority"_a = 0, "weight"_a = 0, "name"_a = "")

        .def("add_timed_transition", py::overload_cast<uint_fast64_t const&, double const& , std::string const&>(&GSPNBuilder::addTimedTransition), "Add a timed transition to the GSPN", "priority"_a, "rate"_a, "name"_a = "")
        .def("add_timed_transition", py::overload_cast<uint_fast64_t const&, double const& , boost::optional<uint64_t> const&, std::string const&>(&GSPNBuilder::addTimedTransition), "Add a timed transition to the GSPN", "priority"_a, "rate"_a, "numServers"_a, "name"_a = "")
        .def("set_transition_layout_info", &GSPNBuilder::setTransitionLayoutInfo, "Set transition layout information", "transitionId"_a, "layoutInfo"_a)

        .def("add_input_arc", py::overload_cast<uint_fast64_t const&, uint_fast64_t const&, uint_fast64_t const&>(&GSPNBuilder::addInputArc), "from"_a , "to"_a,  "multiplicity"_a = 1, "Add a new input arc from a place to an transition")
        .def("add_input_arc", py::overload_cast<std::string const&, std::string const&, uint64_t>(&GSPNBuilder::addInputArc), "from"_a,  "to"_a, "multiplicity"_a = 1, "Add a new input arc from a place to an transition")
        .def("add_inhibition_arc", py::overload_cast<uint_fast64_t const&, uint_fast64_t const&, uint_fast64_t const&>(&GSPNBuilder::addInhibitionArc), "from"_a,  "to"_a,  "multiplicity"_a = 1, "Add an new input arc from a place to an transition")
        .def("add_inhibition_arc", py::overload_cast<std::string const&, std::string const&, uint64_t>(&GSPNBuilder::addInhibitionArc), "from"_a,  "to"_a,  "multiplicity"_a = 1, "Add a new input arc from a place to an transition")
        .def("add_output_arc", py::overload_cast<uint_fast64_t const&, uint_fast64_t const&, uint_fast64_t const&>(&GSPNBuilder::addOutputArc), "from"_a,  "to"_a,  "multiplicity"_a = 1, "Add a new output arc from a place to an transition")
        .def("add_output_arc", py::overload_cast<std::string const&, std::string const&, uint64_t>(&GSPNBuilder::addOutputArc), "from"_a,  "to"_a,  "multiplicity"_a)
        .def("add_normal_arc", &GSPNBuilder::addNormalArc, "Add normal arc from a named element to a named element", "from"_a, "to"_a, "multiplicity"_a = 1)

        .def("build_gspn", &GSPNBuilder::buildGspn, "Construct GSPN", "exprManager"_a = nullptr, "constantsSubstitution"_a = std::map<storm::expressions::Variable, storm::expressions::Expression>())
    ;

    // GspnParser class
    py::class_<GSPNParser, std::shared_ptr<GSPNParser>>(m, "GSPNParser")
         .def(py::init<>())
         .def("parse", [](GSPNParser& p, std::string const& filename, std::string const& constantDefinitions) -> GSPN& {return *(p.parse(filename,constantDefinitions)); }, "filename"_a,  "constantDefinitions"_a = "")
    ;

    // GSPN class
    py::class_<GSPN, std::shared_ptr<GSPN>>(m, "GSPN", "Generalized Stochastic Petri Net")
        // Constructor
        .def(py::init<std::string const& , std::vector<Place> const& , std::vector<ImmediateTransition> const& ,
                     std::vector<TimedTransition> const& , std::vector<TransitionPartition> const& , std::shared_ptr<storm::expressions::ExpressionManager> const& , std::map<storm::expressions::Variable,
                     storm::expressions::Expression> const& > (), "name"_a, "places"_a, "itransitions"_a, "ttransitions"_a, "partitions"_a, "exprManager"_a, "constantsSubstitution"_a = std::map<storm::expressions::Variable, storm::expressions::Expression>())

        .def("get_name", &GSPN::getName, "Get name of GSPN")
        .def("set_name", &GSPN::setName, "Set name of GSPN")

        .def("get_number_of_places", &GSPN::getNumberOfPlaces, "Get the number of places in this GSPN")
        .def("get_number_of_immediate_transitions", &GSPN::getNumberOfImmediateTransitions, "Get the number of immediate transitions in this GSPN")
        .def("get_number_of_timed_transitions", &GSPN::getNumberOfTimedTransitions, "Get the number of timed transitions in this GSPN")

        .def("get_place", [](GSPN const& g, uint64_t id) -> const Place& {return *(g.getPlace(id)); }, "id"_a, "Returns the place with the corresponding id")
        .def("get_place", [](GSPN const& g, std::string const& name) -> const Place& {return *(g.getPlace(name)); }, "name"_a, "Returns the place with the corresponding name")
        .def("get_timed_transition", [](GSPN const& g, std::string const& name) -> const TimedTransition& {return *(g.getTimedTransition(name)); }, "name"_a, "Returns the timed transition with the corresponding name")
        .def("get_immediate_transition", [](GSPN const& g, std::string const& name) -> const ImmediateTransition& {return *(g.getImmediateTransition(name)); }, "name"_a, "Returns the immediate transition with the corresponding name")
        .def("get_transition",[](GSPN const& g, std::string const& name) -> const Transition& {return *(g.getTransition(name)); }, "name"_a, "Returns the transition with the corresponding name")

        .def("get_partitions", &GSPN::getPartitions, "Get the partitions of this GSPN")
        .def("get_places", &GSPN::getPlaces, "Get the places of this GSPN")
        .def("get_timed_transitions", &GSPN::getTimedTransitions, "Get the timed transitions of this GSPN")
        .def("get_immediate_transitions", &GSPN::getImmediateTransitions, "Get the immediate transitions of this GSPN")
        .def("get_initial_marking", &GSPN::getInitialMarking, "Computes the initial marking of this GSPN")

        .def("is_valid", &GSPN::isValid, "Perform some checks")

        // GSPN export
        .def("export_gspn_pnpro_file", [](GSPN& g, std::string const& filepath) -> void { gspnToFile(g, filepath, "to-pnpro"); }, "filepath"_a, "Export GSPN to PNPRO file")
        .def("export_gspn_pnml_file", [](GSPN& g, std::string const& filepath) -> void { gspnToFile(g, filepath, "to-pnml"); }, "filepath"_a, "Export GSPN to PNML file")

        .def_static("timed_transition_id_to_transition_id", &GSPN::timedTransitionIdToTransitionId)
        .def_static("immediate_transition_id_to_transition_id", &GSPN::immediateTransitionIdToTransitionId)
        .def_static("transition_id_to_timed_transition_id", &GSPN::transitionIdToTimedTransitionId)
        .def_static("transition_id_to_immediate_transition_id", &GSPN::transitionIdToImmediateTransitionId)
    ;


    // LayoutInfo class
    py::class_<LayoutInfo>(m, "LayoutInfo")
        .def(py::init<>())
        .def(py::init<double, double, double>(), "x"_a, "y"_a, "rotation"_a = 0.0)
        .def_readwrite("x", &LayoutInfo::x)
        .def_readwrite("y", &LayoutInfo::y)
        .def_readwrite("rotation", &LayoutInfo::rotation)
    ;

    // Place class
    py::class_<Place, std::shared_ptr<Place>>(m, "Place", "Place in a GSPN")
        .def(py::init<uint64_t>(), "id"_a)
        .def("get_name", &Place::getName, "Get name of this place")
        .def("set_name", &Place::setName, "name"_a, "Set name of this place")
        .def("get_id", &Place::getID, "Get the id of this place")
        .def("set_number_of_initial_tokens", &Place::setNumberOfInitialTokens, "tokens"_a, "Set the number of initial tokens of this place")
        .def("get_number_of_initial_tokens", &Place::getNumberOfInitialTokens, "Get the number of initial tokens of this place")
        .def("set_capacity", &Place::setCapacity, "cap"_a, "Set the capacity of tokens of this place")  // problem: boost or lambda [](..
        .def("get_capacity", &Place::getCapacity, "Get the capacity of tokens of this place")
        .def("has_restricted_capacity", &Place::hasRestrictedCapacity, "Is capacity of this place restricted")
    ;

    // Transition class
    py::class_<Transition, std::shared_ptr<Transition>>(m, "Transition", "Transition in a GSPN")
        .def(py::init<>())
        .def("get_id", &Transition::getID, "Get id of this transition")
        .def("set_name", &Transition::setName, "name"_a, "Set name of this transition")
        .def("get_name", &Transition::getName, "Get name of this transition")
        .def("set_priority", &Transition::setPriority, "priority"_a, "Set priority of this transition")
        .def("get_priority", &Transition::getPriority, "Get priority of this transition")

        .def("set_input_arc_multiplicity", &Transition::setInputArcMultiplicity, "place"_a, "multiplicity"_a, "Set the multiplicity of the input arc originating from the place.")
        .def("remove_input_arc", &Transition::removeInputArc, "place"_a, "Remove an input arc connected to a given place.")
        .def("exists_input_arc", &Transition::existsInputArc, "place"_a, "Check whether the given place is connected to this transition via an input arc.")

        .def("set_output_arc_multiplicity", &Transition::setOutputArcMultiplicity, "place"_a, "multiplicity"_a, "Set the multiplicity of the output arc going to the place.")
        .def("remove_output_arc", &Transition::removeOutputArc, "place"_a, "Remove an output arc connected to a given place.")
        .def("exists_output_arc", &Transition::existsOutputArc, "place"_a, "Check whether the given place is connected to this transition via an output arc.")

        .def("set_inhibition_arc_multiplicity", &Transition::setInhibitionArcMultiplicity,  "place"_a, "multiplicity"_a, "Set the multiplicity of the inhibition arc originating from the place.")
        .def("remove_inhibition_arc", &Transition::removeInhibitionArc, "place"_a, "Remove an inhibition arc connected to a given place.")
        .def("exists_inhibition_arc", &Transition::existsInhibitionArc, "place"_a, "Check whether the given place is connected to this transition via an inhibition arc.")

        .def("get_input_places", &Transition::getInputPlaces)
        .def("get_output_places", &Transition::getOutputPlaces)
        .def("get_inhibition_places", &Transition::getInhibitionPlaces)

        .def("get_input_arc_multiplicity", &Transition::getInputArcMultiplicity, "place"_a, "Returns the corresponding multiplicity.")
        .def("get_inhibition_arc_multiplicity", &Transition::getInhibitionArcMultiplicity, "place"_a, "Returns the corresponding multiplicity.")
        .def("get_output_arc_multiplicity", &Transition::getOutputArcMultiplicity, "place"_a, "Returns the corresponding multiplicity.")

        .def("is_enabled", &Transition::isEnabled, "marking"_a, "Check if the given marking enables the transition.")
        .def("fire", &Transition::fire, "marking"_a, "Fire the transition if possible.")

    ;

    // TimedTransition class
    py::class_<TimedTransition, Transition, std::shared_ptr<TimedTransition>>(m, "TimedTransition", "TimedTransition in a GSPN")
        .def(py::init<>())
        .def("get_rate", &TimedTransition::getRate, "Get rate of this transition")
        .def("set_rate", &TimedTransition::setRate, "rate"_a, "Set rate of this transition")
        .def("set_k_server_semantics", &TimedTransition::setKServerSemantics, "k"_a, "Set semantics of this transition")
        .def("set_single_server_semantics", &TimedTransition::setSingleServerSemantics, "Set semantics of this transition")
        .def("set_infinite_server_semantics", &TimedTransition::setInfiniteServerSemantics, "Set semantics of this transition")
        .def("has_k_server_semantics", &TimedTransition::hasKServerSemantics, "Get semantics of this transition")
        .def("has_single_server_semantics", &TimedTransition::hasSingleServerSemantics, "Get semantics of this transition")
        .def("has_infinite_server_semantics", &TimedTransition::hasInfiniteServerSemantics, "Get semantics of this transition")
        .def("get_number_of_servers", &TimedTransition::getNumberOfServers, "Get number of servers")
    ;

    // ImmediateTransition class
    py::class_<ImmediateTransition, Transition, std::shared_ptr<ImmediateTransition>>(m, "ImmediateTransition", "ImmediateTransition in a GSPN")
        .def(py::init<>())
        .def("get_weight", &ImmediateTransition::getWeight, "Get weight of this transition")
        .def("set_weight", &ImmediateTransition::setWeight, "weight"_a, "Set weight of this transition")
        .def("no_weight_attached", &ImmediateTransition::noWeightAttached, "True iff no weight is attached")
    ;

    // TransitionPartition class
    py::class_<TransitionPartition>(m, "TransitionPartition")
         .def(py::init<>())
         .def_readwrite("priority", &TransitionPartition::priority)
         .def_readwrite("transitions", &TransitionPartition::transitions)
         .def("nr_transitions", &TransitionPartition::nrTransitions, "Get number of transitions")
    ;

}
