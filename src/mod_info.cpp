#include "common.h"
#include "storm/utility/storm-version.h"

PYBIND11_MODULE(info, m) {
    m.doc() = "Storm information";

#ifdef STORMPY_DISABLE_SIGNATURE_DOC
    py::options options;
    options.disable_function_signatures();
#endif

    py::class_<storm::utility::StormVersion>(m, "Version", "Version information for Storm")
        // static properties are still called with self as argument (which we ignore), see
        // https://pybind11.readthedocs.io/en/master/advanced/classes.html#static-properties
        .def_property_readonly_static("major", [](py::object /* self */){ return storm::utility::StormVersion::versionMajor; }, "Storm major version.")
        .def_property_readonly_static("minor", [](py::object /* self */){ return storm::utility::StormVersion::versionMinor; }, "Storm minor version.")
        .def_property_readonly_static("patch", [](py::object /* self */){ return storm::utility::StormVersion::versionPatch; }, "Storm patch version.")
        .def_property_readonly_static("development", [](py::object /* self */){ return storm::utility::StormVersion::versionDev; }, "Flag if Storm is development version.")
        .def_property_readonly_static("short", [](py::object /* self */){ return storm::utility::StormVersion::shortVersionString(); }, "Storm version in short representation.")
        .def_property_readonly_static("long", [](py::object /* self */){ return storm::utility::StormVersion::longVersionString(); }, "Storm version in long representation.")
        .def_property_readonly_static("build_info", [](py::object /* self */){ return storm::utility::StormVersion::buildInfo(); }, "Build info for Storm.")
    ;
}
