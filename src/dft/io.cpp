#include "io.h"
#include "src/helpers.h"


// Define python bindings
void define_input(py::module& m) {

    // Load DFT input
    m.def("load_dft_galileo_file", &storm::api::loadDFTGalileoFile<double>, "Load DFT from Galileo file", py::arg("path"));
    // Parse Jani model
    m.def("load_dft_json_file", &storm::api::loadDFTJsonFile<double>, "Load DFT from JSON file", py::arg("path"));
    m.def("load_dft_json_string", &storm::api::loadDFTJsonString<double>, "Load DFT from JSON string", py::arg("json_string"));
}

void define_output(py::module& m) {

    // Export DFT
    m.def("export_dft_json_file", &storm::api::exportDFTToJsonFile<double>, "Export DFT to JSON file", py::arg("dft"), py::arg("path"));
    m.def("export_dft_json_string", &storm::api::exportDFTToJsonString<double>, "Export DFT to JSON string", py::arg("dft"));
}
