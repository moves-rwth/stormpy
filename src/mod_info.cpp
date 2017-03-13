#include "common.h"
#include "storm/utility/storm-version.h"

PYBIND11_PLUGIN(info) {
	py::module m("info", "Storm information");
    py::class_<storm::utility::StormVersion>(m, "Version", "Version information for Storm")
        // static properties are still called with self as argument (which we ignore), see
        // https://pybind11.readthedocs.io/en/master/advanced/classes.html#static-properties
        .def_property_readonly_static("short", [](py::object /* self */){ return storm::utility::StormVersion::shortVersionString(); }, "Storm version in short representation")
        .def_property_readonly_static("long", [](py::object /* self */){ return storm::utility::StormVersion::longVersionString(); }, "Storm version in long representation")
        .def_property_readonly_static("build_info", [](py::object /* self */){ return storm::utility::StormVersion::buildInfo(); }, "Build info for Storm")
    ;
	return m.ptr();
}
