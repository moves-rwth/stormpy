#pragma once

#include "common.h"

void define_simulator(py::module& m);

template<typename ValueType>
void define_simulator_typed(py::module& m, std::string const& vt_suffix);
