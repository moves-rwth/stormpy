/*
 * constraint.h
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#ifndef PYTHON_FORMULA_CONSTRAINT_H_
#define PYTHON_FORMULA_CONSTRAINT_H_

#include "src/common.h"

void define_relation(py::module& m);
void define_constraint(py::module& m);
void define_simple_constraint(py::module& m);

#endif /* PYTHON_FORMULA_CONSTRAINT_H_ */
