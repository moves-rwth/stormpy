/*
 * rational.h
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#ifndef PYTHON_CORE_RATIONAL_H_
#define PYTHON_CORE_RATIONAL_H_

#include "src/common.h"

void define_cln_rational(py::module& m);
void define_gmp_rational(py::module& m);

#endif /* PYTHON_CORE_RATIONAL_H_ */
