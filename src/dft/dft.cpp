#include "dft.h"
#include "src/helpers.h"
#include "storm-dft/storage/DFT.h"
#include "storm/settings/SettingsManager.h"
#include "storm-dft/utility/DftModularizer.h"
#include "storm-dft/settings/modules/FaultTreeSettings.h"
#include "storm-dft/settings/modules/DftIOSettings.h"
#include "storm-dft/storage/DFTIsomorphism.h"


template<typename ValueType> using DFT = storm::dft::storage::DFT<ValueType>;
template<typename ValueType> using DFTElement = storm::dft::storage::elements::DFTElement<ValueType>;
using DFTSymmetries = storm::dft::storage::DFTIndependentSymmetries;

// requires pycarl.Variable
std::set<storm::RationalFunctionVariable> getParameters(DFT<storm::RationalFunction> const& dft) {
    return storm::dft::storage::getParameters(dft);
}

void define_dft(py::module& m) {

    m.def("_set_up", []() {
            storm::settings::addModule<storm::dft::settings::modules::FaultTreeSettings>();
            storm::settings::addModule<storm::dft::settings::modules::DftIOSettings>();
        }, "Initialize Storm-dft");

    m.def("get_parameters", &getParameters, "Collect parameters in parametric DFT", py::arg("dft"));
}

template<typename ValueType>
void define_dft_typed(py::module& m, std::string const& vt_suffix) {
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
        .def("modules", [](DFT<ValueType> const& dft) {
                storm::dft::utility::DftModularizer<ValueType> modularizer;
                return modularizer.computeModules(dft);
            }, "Compute independent modules of DFT")
        .def("symmetries", [](DFT<ValueType>& dft) {
                return dft.findSymmetries(dft.colourDFT());
            }, "Compute symmetries in DFT")
        .def("state_generation_info", &DFT<ValueType>::buildStateGenerationInfo, py::arg("symmetries"), "Build state generation information")
        .def("set_relevant_events", &DFT<ValueType>::setRelevantEvents, py::arg("relevant_events"), py::arg("allow_dc_for_revelant"))
    ;
}


void define_symmetries(py::module& m) {

    py::class_<DFTSymmetries, std::shared_ptr<DFTSymmetries>>(m, "DFTSymmetries", "Symmetries in DFT")
        .def(py::init([]() {
                std::map<size_t, std::vector<std::vector<size_t>>> emptySymmetry;
                return DFTSymmetries(emptySymmetry);
            }), "Constructor for empty symmetry")
        .def_readonly("groups", &DFTSymmetries::groups, "Symmetry groups")
        .def("__str__", &streamToString<DFTSymmetries>)
    ;
}


template void define_dft_typed<double>(py::module& m, std::string const& vt_suffix);
template void define_dft_typed<storm::RationalFunction>(py::module& m, std::string const& vt_suffix);
