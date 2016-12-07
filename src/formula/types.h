/*
 * types.h
 *
 *  Created on: 16 Apr 2016
 *      Author: harold
 */

#ifndef PYTHON_FORMULA_TYPES_H_
#define PYTHON_FORMULA_TYPES_H_

#include "core/common.h"

#include <carl/formula/Constraint.h>
#include <carl/core/SimpleConstraint.h>
#include <carl/formula/Formula.h>

typedef carl::Constraint<Polynomial> Constraint;
typedef carl::SimpleConstraint<Polynomial> SimpleConstraint;
typedef carl::SimpleConstraint<FactorizedRationalFunction> SimpleConstraintRatFunc;
typedef carl::Formula<Polynomial> Formula;

#endif /* PYTHON_FORMULA_TYPES_H_ */
