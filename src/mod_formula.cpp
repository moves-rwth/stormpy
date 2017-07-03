#include "common.h"
#include "helpers.h"

#include "formula/formula_type.h"
#include "formula/relation.h"

PYBIND11_MODULE(formula, m) {
	m.doc() = "pycarl formula untyped functions";


	// Constraint relies on Rational
	m.import("pycarl.core");

	define_relation(m);
	define_formula_type(m);

}
