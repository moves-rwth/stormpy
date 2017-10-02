#include "core.h"
#include "storm/utility/initialize.h"
#include "storm/utility/DirectEncodingExporter.h"
#include "storm/storage/ModelFormulasPair.h"

void define_core(py::module& m) {
    // Init
    m.def("_set_up", [](std::string const& args) {
            storm::utility::setUp();
            storm::settings::initializeAll("StoRM-Py", "stormpy");
            storm::settings::SettingsManager::manager().setFromString(args);
        }, "Initialize Storm", py::arg("arguments"));
}

void define_parse(py::module& m) {
    // Parse formulas
    m.def("parse_properties", [](std::string const& inputString, boost::optional<std::set<std::string>> const& propertyFilter = boost::none) {
                return storm::api::parseProperties(inputString, propertyFilter);
            }, R"dox(
          
          Parse properties given in the prism format.
          
          :param str formula_str: A string of formulas
          :param str property_filter: A filter
          :return: A list of properties
          )dox", py::arg("formula_string"), py::arg("property_filter") = boost::none);
    m.def("parse_properties_for_prism_program", &storm::api::parsePropertiesForPrismProgram,  R"dox(
          
          Parses properties given in the prism format, allows references to variables in the prism program.
          
          :param str formula_str: A string of formulas
          :param PrismProgram prism_program: A prism program
          :param str property_filter: A filter
          :return: A list of properties
          )dox", py::arg("formula_string"), py::arg("prism_program"), py::arg("property_filter") = boost::none);

    // Pair <Model,Formulas>
    py::class_<storm::storage::ModelFormulasPair>(m, "ModelFormulasPair", "Pair of model and formulas")
        .def_property_readonly("model", [](storm::storage::ModelFormulasPair const& pair) {
                return pair.model;
            }, "The model")
        .def_property_readonly("formulas", [](storm::storage::ModelFormulasPair const& pair) {
                return pair.formulas;
            }, "The formulas")
    ;
}

// Thin wrapper for model building using one formula as argument
template<typename ValueType>
std::shared_ptr<storm::models::ModelBase> buildSparseModel(storm::storage::SymbolicModelDescription const& modelDescription, std::vector<std::shared_ptr<storm::logic::Formula const>> const& formulas, bool jit = false, bool doctor = false) {
    if (formulas.empty()) {
        // Build all labels and rewards
        storm::builder::BuilderOptions options(true, true);
        return storm::api::buildSparseModel<ValueType>(modelDescription, options, jit, doctor);
    } else {
        // Only build labels necessary for formulas
        return storm::api::buildSparseModel<ValueType>(modelDescription, formulas, jit, doctor);
    }
}

void define_build(py::module& m) {
    // Build model
    m.def("_build_sparse_model_from_prism_program", &buildSparseModel<double>, "Build the model", py::arg("model_description"), py::arg("formulas") = std::vector<std::shared_ptr<storm::logic::Formula const>>(), py::arg("use_jit") = false, py::arg("doctor") = false);
    m.def("_build_sparse_parametric_model_from_prism_program", &buildSparseModel<storm::RationalFunction>, "Build the parametric model", py::arg("model_description"), py::arg("formulas") = std::vector<std::shared_ptr<storm::logic::Formula const>>(), py::arg("use_jit") = false, py::arg("doctor") = false);
    m.def("_build_sparse_model_from_drn", &storm::api::buildExplicitDRNModel<double>, "Build the model from DRN", py::arg("file"));
    m.def("_build_sparse_parametric_model_from_drn", &storm::api::buildExplicitDRNModel<storm::RationalFunction>, "Build the parametric model from DRN", py::arg("file"));
    m.def("build_sparse_model_from_explicit", &storm::api::buildExplicitModel<double>, "Build the model model from explicit input", py::arg("transition_file"), py::arg("labeling_file"), py::arg("state_reward_file") = "", py::arg("transition_reward_file") = "", py::arg("choice_labeling_file") = "");

}

// Thin wrapper for exporting model
template<typename ValueType>
void exportDRN(std::shared_ptr<storm::models::sparse::Model<ValueType>> model, std::string const& file) {
     std::ofstream stream;
    storm::utility::openFile(file, stream);
    storm::exporter::explicitExportSparseModel(stream, model, {});
    storm::utility::closeFile(stream);
}

void define_export(py::module& m) {
    // Export
    m.def("export_to_drn", &exportDRN<double>, "Export model in DRN format", py::arg("model"), py::arg("file"));
    m.def("export_parametric_to_drn", &exportDRN<storm::RationalFunction>, "Export parametric model in DRN format", py::arg("model"), py::arg("file"));
}
