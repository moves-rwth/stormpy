#ifndef PYTHON_STORAGE_MATRIX_H_
#define PYTHON_STORAGE_MATRIX_H_

#include "common.h"

template<typename ValueType>
void define_sparse_matrix(py::module& m, std::string const& vtSuffix);

void define_sparse_matrix_nt(py::module& m);

#endif /* PYTHON_STORAGE_MATRIX_H_ */
