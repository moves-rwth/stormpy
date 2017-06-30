#include "pars.h"
#include "storm/settings/SettingsManager.h"
#include "storm-pars/settings/modules/ParametricSettings.h"
#include "storm-pars/settings/modules/RegionSettings.h"

void define_pars(py::module& m) {
    m.def("_set_up", []() {
            storm::settings::addModule<storm::settings::modules::ParametricSettings>();
            storm::settings::addModule<storm::settings::modules::RegionSettings>();
        }, "Initialize Storm-pars");
}
