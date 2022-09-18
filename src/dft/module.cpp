#include "dft.h"
#include "src/helpers.h"
#include "storm-dft/storage/DftModule.h"

using DftIndependentModule = storm::dft::storage::DftIndependentModule;

void define_module(py::module& m) {

    py::class_<DftIndependentModule, std::shared_ptr<DftIndependentModule>>(m, "DftIndependentModule", "Independent module in DFT")
        .def("static", &DftIndependentModule::isStatic, "Whether the module contains only static elements (except in submodules)")
        .def("fully_static", &DftIndependentModule::isFullyStatic, "Whether the module contains only static elements (also in submodules)")
        .def("representative", &DftIndependentModule::getRepresentative, "Get module representative")
        .def("elements", &DftIndependentModule::getElements, "Get elements of module (excluding submodules)")
        .def("submodules", &DftIndependentModule::getSubModules, "Get submodules")
        .def("_subtree_double", &DftIndependentModule::getSubtree<double>, "Get subtree formed by module", py::arg("dft"))
        .def("_subtree_ratfunc", &DftIndependentModule::getSubtree<storm::RationalFunction>, "Get subtree formed by module", py::arg("dft"))
    ;
}
