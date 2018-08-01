#include "dft.h"
#include "src/helpers.h"
#include "storm-dft/storage/dft/DFT.h"
#include "storm/settings/SettingsManager.h"
#include "storm-dft/settings/modules/FaultTreeSettings.h"


template<typename ValueType> using DFT = storm::storage::DFT<ValueType>;


void define_dft(py::module& m) {

    m.def("_set_up", []() {
            storm::settings::addModule<storm::settings::modules::FaultTreeSettings>();
        }, "Initialize Storm-dft");


    // DFT class
    py::class_<DFT<double>, std::shared_ptr<DFT<double>>>(m, "DFT", "DFT")
        .def("nr_elements", &DFT<double>::nrElements)
    ;

    py::class_<DFT<storm::RationalFunction>, std::shared_ptr<DFT<storm::RationalFunction>>>(m, "ParametricDFT", "Parametric DFT")
        .def("nr_elements", &DFT<storm::RationalFunction>::nrElements)
    ;

}
