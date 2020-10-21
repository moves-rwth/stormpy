#ifndef PYTHON_STORAGE_SPARSEMODELCOMPONENTS_H
#define PYTHON_STORAGE_SPARSEMODELCOMPONENTS_H

#include "common.h"

template<typename ValueType>
void define_sparse_model_components(py::module& m, std::string const& vtSuffix);

#endif /* PYTHON_STORAGE_SPARSEMODELCOMPONENTS_H */