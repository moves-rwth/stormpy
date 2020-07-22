#include "dft.h"
#include "src/helpers.h"
#include "storm-dft/storage/dft/DFT.h"
#include "storm/settings/SettingsManager.h"
#include "storm-dft/settings/modules/FaultTreeSettings.h"
#include "storm-dft/settings/modules/DftIOSettings.h"
#include "storm-dft/storage/dft/DFTIsomorphism.h"


template<typename ValueType> using DFT = storm::storage::DFT<ValueType>;
template<typename ValueType> using DFTElement = storm::storage::DFTElement<ValueType>;
using DFTSymmetries = storm::storage::DFTIndependentSymmetries;


void define_dft(py::module& m) {

    m.def("_set_up", []() {
            storm::settings::addModule<storm::settings::modules::FaultTreeSettings>();
            storm::settings::addModule<storm::settings::modules::DftIOSettings>();
        }, "Initialize Storm-dft");

    // DFT element
    py::class_<DFTElement<double>, std::shared_ptr<DFTElement<double>>>(m, "DFTElement", "DFT element")
        .def_property_readonly("id", &DFTElement<double>::id, "Id")
        .def_property_readonly("name", &DFTElement<double>::name, "Name")
        .def("__str__", &DFTElement<double>::toString)
    ;

    py::class_<DFTElement<storm::RationalFunction>, std::shared_ptr<DFTElement<storm::RationalFunction>>>(m, "ParametricDFTElement", "Parametric DFT element")
        .def_property_readonly("id", &DFTElement<storm::RationalFunction>::id, "Id")
        .def_property_readonly("name", &DFTElement<storm::RationalFunction>::name, "Name")
        .def("__str__", &DFTElement<storm::RationalFunction>::toString)
    ;

    // DFT class
    py::class_<DFT<double>, std::shared_ptr<DFT<double>>>(m, "DFT", "Dynamic Fault Tree")
        .def("nr_elements", &DFT<double>::nrElements, "Total number of elements")
        .def("nr_be", &DFT<double>::nrBasicElements, "Number of basic elements")
        .def("nr_dynamic", &DFT<double>::nrDynamicElements, "Number of dynamic elements")
        .def("can_have_nondeterminism", &DFT<double>::canHaveNondeterminism, "Whether the model can contain non-deterministic choices")
        .def("__str__", &DFT<double>::getInfoString)
        .def_property_readonly("top_level_element", [](DFT<double>& dft) {
                return dft.getElement(dft.getTopLevelIndex());
            }, "Get top level element")
        .def("get_element", &DFT<double>::getElement, "Get DFT element at index", py::arg("index"))
        .def("get_element_by_name", [](DFT<double>& dft, std::string const& name) {
                return dft.getElement(dft.getIndex(name));
            }, "Get DFT element by name", py::arg("name"))
        .def("modularisation", &DFT<double>::topModularisation, "Split DFT into independent modules")
        .def("symmetries", [](DFT<double>& dft) {
                return dft.findSymmetries(dft.colourDFT());
            }, "Compute symmetries in DFT")
    ;

    py::class_<DFT<storm::RationalFunction>, std::shared_ptr<DFT<storm::RationalFunction>>>(m, "ParametricDFT", "Parametric DFT")
        .def("nr_elements", &DFT<storm::RationalFunction>::nrElements, "Total number of elements")
        .def("nr_be", &DFT<storm::RationalFunction>::nrBasicElements, "Number of basic elements")
        .def("nr_dynamic", &DFT<storm::RationalFunction>::nrDynamicElements, "Number of dynamic elements")
        .def("can_have_nondeterminism", &DFT<storm::RationalFunction>::canHaveNondeterminism, "Whether the model can contain non-deterministic choices")
        .def("__str__", &DFT<storm::RationalFunction>::getInfoString)
        .def_property_readonly("top_level_element", [](DFT<storm::RationalFunction>& dft) {
                return dft.getElement(dft.getTopLevelIndex());
            }, "Get top level element")
        .def("get_element", &DFT<storm::RationalFunction>::getElement, "Get DFT element at index", py::arg("index"))
        .def("get_element_by_name", [](DFT<storm::RationalFunction>& dft, std::string const& name) {
                return dft.getElement(dft.getIndex(name));
            }, "Get DFT element by name", py::arg("name"))
        .def("modularisation", &DFT<storm::RationalFunction>::topModularisation, "Split DFT into independent modules")
        .def("symmetries", [](DFT<storm::RationalFunction>& dft) {
                return dft.findSymmetries(dft.colourDFT());
            }, "Compute symmetries in DFT")
    ;

}


void define_symmetries(py::module& m) {

    py::class_<DFTSymmetries, std::shared_ptr<DFTSymmetries>>(m, "DFTSymmetries", "Symmetries in DFT")
        .def_readonly("groups", &DFTSymmetries::groups, "Symmetry groups")
        .def("__str__", &streamToString<DFTSymmetries>)
    ;
}
