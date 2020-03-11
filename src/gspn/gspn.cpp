#include "gspn.h"
#include "src/helpers.h"
#include "storm-gspn/storage/gspn/GSPN.h"
#include "storm-gspn/storage/gspn/GspnBuilder.h"
#include "storm/settings/SettingsManager.h"


using GSPN = storm::gspn::GSPN;
using GSPNBuilder = storm::gspn::GspnBuilder;
using LayoutInfo = storm::gspn::LayoutInfo;
using Place = storm::gspn::Place;
using TimedTransition = storm::gspn::TimedTransition<GSPN::RateType>;
using ImmediateTransition = storm::gspn::ImmediateTransition<GSPN::WeightType>;
using Transition = storm::gspn::Transition;


void define_gspn(py::module& m) {

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
        .def("set_capacity", &Place::setCapacity, "cap"_a, "Set the capacity of tokens of this place")
        .def("get_capacity", &Place::getCapacity, "Get the capacity of tokens of this place")
        .def("has_restricted_capacity", &Place::hasRestrictedCapacity, "Is capacity of this place restricted")
    ;

    // Transition class
    py::class_<Transition, std::shared_ptr<Transition>>(m, "Transition", "Transition in a GSPN")
        .def(py::init<>())
        .def("get_name", &Transition::getName, "Get name of this transition")
        .def("set_name", &Transition::setName, "name"_a, "Set name of this transition")
        // todo add missing
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

    // GSPN class
    py::class_<GSPN, std::shared_ptr<GSPN>>(m, "GSPN", "Generalized Stochastic Petri Net")
        .def("name", &GSPN::getName, "Name of GSPN")
        .def("set_name", &GSPN::setName, "Set name of GSPN")

        // todo constructor:
        //  .def(py::init<std::string const&, std::vector<Place>, ... >())

        // todo write tests:
        .def_static("timed_transition_id_to_transition_id", &GSPN::timedTransitionIdToTransitionId)
        .def_static("immediate_transition_id_to_transition_id", &GSPN::immediateTransitionIdToTransitionId)
        .def_static("transition_id_to_timed_transition_id", &GSPN::transitionIdToTimedTransitionId)
        .def_static("transition_id_to_immediate_transition_id", &GSPN::transitionIdToImmediateTransitionId)

        // todo: getNumberOfPlaces, getNumberOfImmediateTransitions, getNumberOfTimedTransitions, ...
        // todo export (see JaniToString)  toPnpro, toPnml

    ;

    // GSPN_Builder class
    py::class_<GSPNBuilder, std::shared_ptr<GSPNBuilder>>(m, "GSPNBuilder", "Generalized Stochastic Petri Net Builder")
            .def(py::init(), "Constructor")
            .def("set_name", &GSPNBuilder::setGspnName, "Set name of GSPN", py::arg("name"))

            // todo: boost::optional<uint64_t> capacity
            //.def("add_place", &GSPNBuilder::addPlace, "Add a place to the GSPN", py::arg("capacity") = 1, py::arg("initialTokens") = 0, py::arg("name") = "")
            //.def("set_place_layout_info", &GSPNBuilder::setPlaceLayoutInfo, "Set place layout information", py::arg("placeId"), py::arg("layoutInfo"))

            .def("add_immediate_transition", &GSPNBuilder::addImmediateTransition, "Adds an immediate transition to the GSPN", py::arg("priority") = 0, py::arg("weight") = 0, py::arg("name") = "")

            // todo: boost::optional<uint64_t> + write tests:
            .def("add_timed_transition", py::overload_cast<uint_fast64_t const&, double const& , std::string const&>(&GSPNBuilder::addTimedTransition), "Adds an timed transition to the GSPN", py::arg("priority"), py::arg("rate"), py::arg("name") = "")
            .def("add_timed_transition", py::overload_cast<uint_fast64_t const&, double const& , boost::optional<uint64_t>, std::string const&>(&GSPNBuilder::addTimedTransition), "Adds an timed transition to the GSPN", py::arg("priority"), py::arg("rate"), py::arg("numServers"), py::arg("name") = "")
            .def("set_transition_layout_info", &GSPNBuilder::setTransitionLayoutInfo, "set transition layout information", py::arg("transitionId"), py::arg("layoutInfo"))

            .def("add_input_arc", py::overload_cast<uint_fast64_t const&, uint_fast64_t const&, uint_fast64_t const&>(&GSPNBuilder::addInputArc), py::arg("from"),  py::arg("to"),  py::arg("multiplicity"))
            .def("add_input_arc", py::overload_cast<std::string const&, std::string const&, uint64_t>(&GSPNBuilder::addInputArc), py::arg("from"),  py::arg("to"),  py::arg("multiplicity"))
            .def("add_inhibition_arc", py::overload_cast<uint_fast64_t const&, uint_fast64_t const&, uint_fast64_t const&>(&GSPNBuilder::addInhibitionArc), py::arg("from"),  py::arg("to"),  py::arg("multiplicity"))
            .def("add_inhibition_arc", py::overload_cast<std::string const&, std::string const&, uint64_t>(&GSPNBuilder::addInhibitionArc), py::arg("from"),  py::arg("to"),  py::arg("multiplicity"))
            .def("add_output_arc", py::overload_cast<uint_fast64_t const&, uint_fast64_t const&, uint_fast64_t const&>(&GSPNBuilder::addOutputArc), py::arg("from"),  py::arg("to"),  py::arg("multiplicity"))
            .def("add_output_arc", py::overload_cast<std::string const&, std::string const&, uint64_t>(&GSPNBuilder::addOutputArc), py::arg("from"),  py::arg("to"),  py::arg("multiplicity"))
            .def("add_normal_arc", &GSPNBuilder::addNormalArc, "Add normal arc from a named element to a named element", py::arg("from"), py::arg("to"), py::arg("multiplicity") = 1)

            .def("build_gspn", &GSPNBuilder::buildGspn, "Construct GSPN", py::arg("exprManager") = nullptr, py::arg("constantsSubstitution") = std::map<storm::expressions::Variable, storm::expressions::Expression>())

    ;
}
