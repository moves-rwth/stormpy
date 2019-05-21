#pragma once

#include "common.h"
#include "storm/storage/dd/DdType.h"

void define_model(py::module& m);
void define_sparse_model(py::module& m);

template<storm::dd::DdType DdType>
void define_symbolic_model(py::module& m, std::string vt_suffix);
