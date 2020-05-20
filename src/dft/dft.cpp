#include "dft.h"
#include "src/helpers.h"
#include "storm-dft/storage/dft/DFT.h"
#include "storm/settings/SettingsManager.h"
#include "storm-dft/settings/modules/FaultTreeSettings.h"
#include "storm-dft/settings/modules/DftIOSettings.h"


template<typename ValueType> using DFT = storm::storage::DFT<ValueType>;


void define_dft(py::module& m) {

    m.def("_set_up", []() {
            storm::settings::addModule<storm::settings::modules::FaultTreeSettings>();
            storm::settings::addModule<storm::settings::modules::DftIOSettings>();
        }, "Initialize Storm-dft");


    // DFT class
    py::class_<DFT<double>, std::shared_ptr<DFT<double>>>(m, "DFT", "Dynamic Fault Tree")
        .def("nr_elements", &DFT<double>::nrElements, "Total number of elements")
        .def("nr_be", &DFT<double>::nrBasicElements, "Number of basic elements")
        .def("nr_dynamic", &DFT<double>::nrDynamicElements, "Number of dynamic elements")
        .def("can_have_nondeterminism", &DFT<double>::canHaveNondeterminism, "Whether the model can contain non-deterministic choices")
        .def("__str__", &DFT<double>::getInfoString)
    ;

    py::class_<DFT<storm::RationalFunction>, std::shared_ptr<DFT<storm::RationalFunction>>>(m, "ParametricDFT", "Parametric DFT")
        .def("nr_elements", &DFT<storm::RationalFunction>::nrElements, "Total number of elements")
        .def("nr_be", &DFT<storm::RationalFunction>::nrBasicElements, "Number of basic elements")
        .def("nr_dynamic", &DFT<storm::RationalFunction>::nrDynamicElements, "Number of dynamic elements")
        .def("can_have_nondeterminism", &DFT<storm::RationalFunction>::canHaveNondeterminism, "Whether the model can contain non-deterministic choices")
        .def("__str__", &DFT<storm::RationalFunction>::getInfoString)
    ;

}
