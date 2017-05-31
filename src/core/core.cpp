#include "core.h"
#include "storm/utility/DirectEncodingExporter.h"

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
    m.def("parse_properties", &storm::parsePropertiesForExplicit, R"dox(
          
          Parse properties given in the prism format.
          
          :param str formula_str: A string of formulas
          :param str property_filter: A filter
          :return: A list of properties
          )dox", py::arg("formula_string"), py::arg("property_filter") = boost::none);
    m.def("parse_properties_for_prism_program", &storm::parsePropertiesForPrismProgram,  R"dox(
          
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

    // Parse explicit models
    m.def("parse_explicit_model", &storm::parser::AutoParser<>::parseModel, "Parse explicit model", py::arg("transition_file"), py::arg("labeling_file"), py::arg("state_reward_file") = "", py::arg("transition_reward_file") = "", py::arg("choice_labeling_file") = "");
}

// Thin wrapper for model building using one formula as argument
template<typename ValueType>
std::shared_ptr<storm::models::ModelBase> buildSymbolicModel(storm::prism::Program const& program, std::shared_ptr<storm::logic::Formula const> const& formula) {
    return storm::buildSymbolicModel<ValueType>(program, std::vector<std::shared_ptr<storm::logic::Formula const>>(1,formula));
}

template<typename ValueType>
std::shared_ptr<storm::models::ModelBase> buildSparseModel(storm::prism::Program const& program, std::shared_ptr<storm::logic::Formula const> const& formula) {
    return storm::buildSparseModel<ValueType>(program, std::vector<std::shared_ptr<storm::logic::Formula const>>(1,formula));
}

void define_build(py::module& m) {
    // Build model
    m.def("_build_sparse_model_from_prism_program", &storm::buildSparseModel<double>, "Build the model", py::arg("program"), py::arg("formulas") = std::vector<std::shared_ptr<storm::logic::Formula const>>());
    m.def("_build_sparse_parametric_model_from_prism_program", &storm::buildSparseModel<storm::RationalFunction>, "Build the parametric model", py::arg("program"), py::arg("formulas"));
    m.def("_build_sparse_model_from_drn", &storm::buildExplicitDRNModel<double>, "Build the model from DRN", py::arg("file"));
    m.def("_build_sparse_parametric_model_from_drn", &storm::buildExplicitDRNModel<storm::RationalFunction>, "Build the parametric model from DRN", py::arg("file"));

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
