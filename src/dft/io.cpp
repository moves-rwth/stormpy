#include "io.h"
#include "src/helpers.h"


// Define python bindings
void define_input(py::module& m) {

    // Load DFT input
    m.def("load_dft_galileo", &storm::api::loadDFTGalileo<double>, "Load DFT from Galileo file", py::arg("path"));
    // Parse Jani model
    m.def("load_dft_json", &storm::api::loadDFTJson<double>, "Load DFT from JSON file", py::arg("path"));
}

void define_output(py::module& m) {

    // Export DFT
    m.def("export_dft_json", &storm::api::exportDFTToJson<double>, "Export DFT to JSON file", py::arg("dft"), py::arg("path"));
}
