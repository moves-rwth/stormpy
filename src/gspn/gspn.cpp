#include "gspn.h"
#include "src/helpers.h"
#include "storm-gspn/storage/gspn/GSPN.h"
#include "storm-gspn/storage/gspn/GspnBuilder.h"
#include "storm/settings/SettingsManager.h"


using GSPN = storm::gspn::GSPN;
using GSPNBuilder = storm::gspn::GspnBuilder;


void define_gspn(py::module& m) {
    // GSPN class
    py::class_<GSPN, std::shared_ptr<GSPN>>(m, "GSPN", "Generalized Stochastic Petri Net")
        .def("name", &GSPN::getName, "Name of GSPN")
        // todo getter
    ;
//}
// todo ? void define_gspn_builder(py::module& m) {
    // GSPN_Builder class
    py::class_<GSPNBuilder, std::shared_ptr<GSPNBuilder>>(m, "GSPNBuilder", "Generalized Stochastic Petri Net Builder")
            .def(py::init(), "Constructor")
            .def("set_name", &GSPNBuilder::setGspnName, "Name of GSPN", py::arg("name"))
            //.def("add_place", &GSPNBuilder::addPlace, "Add a place to the gspn", py::arg("todo"))
            //.def("set_place_layout_info", &GSPNBuilder::setPlaceLayoutInfo, "Layout"py::arg("todo"))
            //addImmediateTransition
            //addTimedTransition
            //addTimedTransition
            //setTransitionLayoutInfo
            //addInputArc todo overloaded fcts
            //addInhibitionArc //todo overloaded fcts
            //addOutputArc //todo overloaded fcts
            //addNormalArc
            .def("build_gspn", &GSPNBuilder::buildGspn, "Construct GSPN", py::arg("exprManager") = nullptr, py::arg("constantsSubstitution") = std::map<storm::expressions::Variable, storm::expressions::Expression>())
            // todo py::arg(...
            // , py::arg("constantsSubstitution") = std::map<storm::expressions::Variable, storm::expressions::Expression>()
            // todo: argument ExpressionManager, map <Variable,Expression> constantsSubstitution (aufrufe auch ohne args)
            ;
}
