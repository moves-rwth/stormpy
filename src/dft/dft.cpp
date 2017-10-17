#include "dft.h"
#include "storm-dft/parser/DFTJsonParser.h"
#include "storm-dft/builder/ExplicitDFTModelBuilder.h"
#include "storm/settings/SettingsManager.h"
#include "storm-dft/settings/modules/FaultTreeSettings.h"
#include "storm-dft/storage/dft/DFTIsomorphism.h"

// Thin wrapper for model building using one formula as argument
template<typename ValueType>
std::shared_ptr<storm::models::sparse::Model<ValueType>> buildModelFromJsonDft(std::string const& jsonDft, bool symred) {
    // Build DFT
    storm::parser::DFTJsonParser<ValueType> parser;
    storm::storage::DFT<ValueType> dft = parser.parseJson(jsonDft);
    // Build model
    std::map<size_t, std::vector<std::vector<size_t>>> emptySymmetry;
    storm::storage::DFTIndependentSymmetries symmetries(emptySymmetry);
    if (symred) {
        auto colouring = dft.colourDFT();
        symmetries = dft.findSymmetries(colouring);
    }
    storm::builder::ExplicitDFTModelBuilder<ValueType> builder(dft, symmetries, true);
    typename storm::builder::ExplicitDFTModelBuilder<ValueType>::LabelOptions labeloptions({}, true);
    builder.buildModel(labeloptions, 0, 0.0);
    return builder.getModel();
}

void define_dft(py::module& m) {
    m.def("_set_up", []() {
            storm::settings::addModule<storm::settings::modules::FaultTreeSettings>();
        }, "Initialize Storm-dft");
    // Build model
    m.def("build_sparse_model_from_json_dft", &buildModelFromJsonDft<double>, "Build the model", py::arg("jsonDft"), py::arg("symred") = false);
    m.def("build_sparse_parametric_model_from_json_dft", &buildModelFromJsonDft<storm::RationalFunction>, "Build the parametric model", py::arg("jsonDft"), py::arg("symred") = false);
}
