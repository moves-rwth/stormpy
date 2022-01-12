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
}

template<typename ValueType>
void define_dft_typed(py::module& m, std::string const& vt_suffix) {
    // DFT element
    py::class_<DFTElement<ValueType>, std::shared_ptr<DFTElement<ValueType>>>(m, ("DFTElement"+vt_suffix).c_str(), "DFT element")
        .def_property_readonly("id", &DFTElement<ValueType>::id, "Id")
        .def_property_readonly("name", &DFTElement<ValueType>::name, "Name")
        .def("__str__", &DFTElement<ValueType>::toString)
    ;

    // DFT class
    py::class_<DFT<ValueType>, std::shared_ptr<DFT<ValueType>>>(m, ("DFT"+vt_suffix).c_str(), "Dynamic Fault Tree")
        .def("nr_elements", &DFT<ValueType>::nrElements, "Total number of elements")
        .def("nr_be", &DFT<ValueType>::nrBasicElements, "Number of basic elements")
        .def("nr_dynamic", &DFT<ValueType>::nrDynamicElements, "Number of dynamic elements")
        .def("can_have_nondeterminism", &DFT<ValueType>::canHaveNondeterminism, "Whether the model can contain non-deterministic choices")
        .def("__str__", &DFT<ValueType>::getInfoString)
        .def_property_readonly("top_level_element", [](DFT<ValueType>& dft) {
                return dft.getElement(dft.getTopLevelIndex());
            }, "Get top level element")
        .def("get_element", &DFT<ValueType>::getElement, "Get DFT element at index", py::arg("index"))
        .def("get_element_by_name", [](DFT<ValueType>& dft, std::string const& name) {
                return dft.getElement(dft.getIndex(name));
            }, "Get DFT element by name", py::arg("name"))
        .def("modularisation", &DFT<ValueType>::topModularisation, "Split DFT into independent modules")
        .def("symmetries", [](DFT<ValueType>& dft) {
                return dft.findSymmetries(dft.colourDFT());
            }, "Compute symmetries in DFT")
    ;
}


void define_symmetries(py::module& m) {

    py::class_<DFTSymmetries, std::shared_ptr<DFTSymmetries>>(m, "DFTSymmetries", "Symmetries in DFT")
        .def("__init__", [](DFTSymmetries &instance) {
                std::map<size_t, std::vector<std::vector<size_t>>> emptySymmetry;
                new (&instance) DFTSymmetries(emptySymmetry);
            }, "Constructor for empty symmetry")
        .def_readonly("groups", &DFTSymmetries::groups, "Symmetry groups")
        .def("__str__", &streamToString<DFTSymmetries>)
    ;
}


template void define_dft_typed<double>(py::module& m, std::string const& vt_suffix);
template void define_dft_typed<storm::RationalFunction>(py::module& m, std::string const& vt_suffix);
