#include "environment.h"
#include "src/helpers.h"
#include "storm/environment/Environment.h"

void define_environment(py::module& m) {
    py::class_<storm::Environment>(m, "Environment", "Environment")
        .def(py::init<>(), "Construct default environment")
    ;
}

