#include "dft.h"
#include "storm-dft/parser/DFTJsonParser.h"
#include "storm-dft/builder/ExplicitDFTModelBuilderApprox.h"
#include "storm/settings/SettingsManager.h"
#include "storm-dft/settings/modules/DFTSettings.h"

// Thin wrapper for model building using one formula as argument
template<typename ValueType>
std::shared_ptr<storm::models::sparse::Model<ValueType>> buildModelFromJsonDft(std::string const& jsonDft) {
    // Build DFT
    storm::parser::DFTJsonParser<ValueType> parser;
    storm::storage::DFT<ValueType> dft = parser.parseJson(jsonDft);
    // Build model
    std::map<size_t, std::vector<std::vector<size_t>>> emptySymmetry;
    storm::storage::DFTIndependentSymmetries symmetries(emptySymmetry);
    storm::builder::ExplicitDFTModelBuilderApprox<ValueType> builder(dft, symmetries, true);
    typename storm::builder::ExplicitDFTModelBuilderApprox<ValueType>::LabelOptions labeloptions({}, true);
    builder.buildModel(labeloptions, 0, 0.0);
    return builder.getModel();
}

void define_dft(py::module& m) {
    m.def("_set_up", []() {
            storm::settings::addModule<storm::settings::modules::DFTSettings>();
        }, "Initialize Storm-dft");
    // Build model
    m.def("build_sparse_model_from_json_dft", &buildModelFromJsonDft<double>, "Build the model", py::arg("jsonDft"));
    m.def("build_sparse_parametric_model_from_json_dft", &buildModelFromJsonDft<storm::RationalFunction>, "Build the parametric model", py::arg("jsonDft"));
}
